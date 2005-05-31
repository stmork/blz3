/*
 *  libxio.c
 *
 *  Copyright (C) Lukas Hejtmanek - January 2004
 *
 *  This file is part of transcode, a linux video stream processing tool
 *      
 *  transcode is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  transcode is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */

#undef PACKAGE
#undef VERSION

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_IBP
#include <lors.h>
#endif
#include <stdio.h>
#include <stdlib.h>
char *strndup(const char *s, size_t n);
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <stdarg.h>

struct xio_handle_t {

	ssize_t (*xio_read_v)(void *stream, void *buf, size_t count);
	ssize_t (*xio_write_v)(void *stream, const void *buf, size_t count);
	int (*xio_close_v)(void *stream);
	off_t (*xio_lseek_v)(void *stream, off_t offset, int whence);
	int (*xio_ftruncate_v)(void *stream, off_t length);
	int (*xio_fstat_v)(void *stream, struct stat *buf);
	
	void *data;
};

#define MAX_HANDLES 256

struct xio_handle_t * _handles[MAX_HANDLES];
int xio_initialized=0;
pthread_mutex_t xio_lock;

#ifdef HAVE_IBP
#define IBP_URI "lors://"

#define BLOCK_SIZE_SHIFT 1024

#define LBONE_PORT 6767
#define LORS_BLOCKSIZE 10*1024
#define LORS_DURATION 3600
#define LORS_COPIES 1
#define LORS_THREADS 1
#define LORS_TIMEOUT 100
#define LORS_SERVERS 1
#define LORS_SIZE    10*1024*1024

struct xio_ibp_handle_t {
	LorsDepotPool 	*dp;
	LorsExnode	*ex;
	off_t           b_pos;
	off_t           begin;
	off_t		end;
	void		*buffer;
        char            *filename;
	char		*lbone_server;
	int		lbone_port;
	int		lors_blocksize;
	int 		lors_duration;
	int		lors_copies;
	int		lors_threads;
	int		lors_timeout;
	int		lors_servers;
	off_t		lors_size;
	int		fill_buffer;
	int		dirty_buffer;
	int 		mode;
};

static void *
ibp_open(const char *uri, int mode, int m)
{
	struct xio_ibp_handle_t *handle;
	int ret;

	if(strncmp(uri, IBP_URI, strlen(IBP_URI)) != 0) {
		errno = EINVAL;
		return (void *)-1;
	}
	uri += strlen(IBP_URI);
	
	handle=(struct xio_ibp_handle_t*)calloc(1,
			                     sizeof(struct xio_ibp_handle_t));

	// environment setup
 	handle->lbone_server = getenv("LBONE_SERVER");
	if(getenv("LBONE_PORT")) {
		handle->lbone_port = atoi(getenv("LBONE_PORT"));
	} else {
		handle->lbone_port = LBONE_PORT;
	}

	if(getenv("LORS_BLOCKSIZE")) {
		handle->lors_blocksize = atoi(getenv("LORS_BLOCKSIZE"));
	} else {
		if(mode & O_WRONLY || mode & O_CREAT)
			handle->lors_blocksize = LORS_BLOCKSIZE;
		else
			handle->lors_blocksize = 128;
	}

	if(getenv("LORS_DURATION")) {
		handle->lors_duration = atoi(getenv("LORS_DURATION"));
	} else {
		handle->lors_duration = LORS_DURATION;
	}

	if(getenv("LORS_COPIES")) {
		handle->lors_copies = atoi(getenv("LORS_COPIES"));
	} else {
		handle->lors_copies = LORS_COPIES;
	}

	if(getenv("LORS_THREADS")) {
		handle->lors_threads = atoi(getenv("LORS_THREADS"));
	} else {
		handle->lors_threads = LORS_THREADS;
	}

	if(getenv("LORS_TIMEOUT")) {
		handle->lors_timeout = atoi(getenv("LORS_TIMEOUT"));
	} else {
		handle->lors_timeout = LORS_TIMEOUT;
	}

	if(getenv("LORS_SERVERS")) {
		handle->lors_servers = atoi(getenv("LORS_SERVERS"));
	} else {
		handle->lors_servers = LORS_SERVERS;
	}

	if(getenv("LORS_SIZE")) {
                handle->lors_servers = atoi(getenv("LORS_SIZE"));
        } else {
                handle->lors_servers = LORS_SIZE;
        }

	if(*uri != '/') {
		// get LBONE_SERVER from URI
		if(strchr(uri, ':')) {
			// port is defined
			handle->lbone_server = strndup(uri, 
					strchr(uri, ':')-uri);
			uri = (char *)(strchr(uri, ':')+1);
			handle->lbone_port = atoi(uri);
		} else {
			// only host
			handle->lbone_server = strndup(uri, 
					(int)(strchr(uri, '/')-uri));
		}
		uri = (char *)(strchr(uri, '/')+1);
	} else {
		uri += 1;
	}

 	if(!strchr(uri, '?')) {
		// only filename
		handle->filename = strdup(uri);
	} else {
		// parse options
	  	handle->filename = strndup(uri, (int)(strchr(uri, '?')-uri));
		uri = strchr(uri, '?')+1;
		while(uri != (char *)1) {
			if(strncmp(uri, "bs", 2) == 0) {
				handle->lors_blocksize = atoi(&uri[3]);
			} else if(strncmp(uri, "duration", 8) == 0) {
				handle->lors_duration = atoi(&uri[9]);
			} else if(strncmp(uri, "copies", 6) == 0) {
				handle->lors_copies = atoi(&uri[7]);
			} else if(strncmp(uri, "threads", 7) == 0) {
				handle->lors_threads = atoi(&uri[8]);
			} else if(strncmp(uri, "timeout", 7) == 0) {
				handle->lors_timeout = atoi(&uri[8]);
			} else if(strncmp(uri, "servers", 7) == 0) {
				handle->lors_servers = atoi(&uri[8]);
			} else if(strncmp(uri, "size", 4) == 0) {
				handle->lors_size = atoi(&uri[5]);
			}
			uri = strchr(uri, '&') + 1;
		}
	}

	handle->lors_blocksize *= BLOCK_SIZE_SHIFT;
	
	handle->mode = mode;
	handle->b_pos = 0;
	handle->begin = 0;
	handle->end = 0;
	handle->dirty_buffer = 0;

	if(mode & O_WRONLY || mode & O_CREAT) {	
		handle->buffer = malloc(handle->lors_blocksize);
	        if(!handle->buffer) {
	                free(handle);
	                errno = EIO;
	                return (void *)-1;
	        }
		ret = lorsGetDepotPool(&handle->dp, handle->lbone_server, 
					handle->lbone_port, NULL, 
			       		handle->lors_servers, NULL, 
					handle->lors_size/(1024*1024)+1, 
					IBP_HARD, 
			       		handle->lors_duration, 
					handle->lors_threads,
					handle->lors_timeout, 
					LORS_CHECKDEPOTS);
		if(ret != LORS_SUCCESS) {
			errno = EIO;
			return (void *)-1;
		}
		ret = lorsExnodeCreate(&handle->ex);
		if (ret != LORS_SUCCESS) {
			errno = EIO;
		        return (void *)-1;
		}
	}
	if(mode & O_RDONLY || !mode) {
		handle->buffer = malloc(handle->lors_blocksize);
		handle->fill_buffer = 1;
		ret = lorsFileDeserialize(&handle->ex, handle->filename, NULL);
		if(ret != LORS_SUCCESS) {
			errno = EIO;
			return (void *)-1;
		}
		ret = lorsUpdateDepotPool(handle->ex, &handle->dp, 
				          handle->lbone_server, 0, 
					  NULL, handle->lors_threads, 
					  handle->lors_timeout, 0);
		if(ret != LORS_SUCCESS) {
			errno = EIO;
                        return (void *)-1;
                }
	}
	return (void *)handle;
}


static int
ibp_flush(void *handle)
{
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)handle;
	int ret;
	LorsSet *set;

	if(hdl->dirty_buffer == 0) {
		return 0;
	}

	ret = lorsQuery(hdl->ex, &set, hdl->begin, hdl->end, 
			LORS_QUERY_REMOVE);
	if(ret != LORS_SUCCESS) {
		errno = EINVAL;
		return -1;
	}

	if(jrb_empty(set->mapping_map)) {
		ret = lorsSetInit(&set, hdl->end/hdl->lors_threads, 1, 0);
                if(ret != LORS_SUCCESS) {
	                errno = EIO;
	                return -1;
	        }

		ret = lorsSetStore(set, hdl->dp, hdl->buffer, 
				hdl->begin, hdl->end, NULL, 
				hdl->lors_threads, 
				hdl->lors_timeout, LORS_RETRY_UNTIL_TIMEOUT);
		
		if(ret != LORS_SUCCESS) {
			lorsSetFree(set,LORS_FREE_MAPPINGS);
			errno = EIO;
			return -1;
		}
	} else {
		set->copies=hdl->lors_copies;
                set->data_blocksize=hdl->end/hdl->lors_threads;
		ret = lorsSetUpdate(set, hdl->dp, hdl->buffer, 
				    hdl->begin, hdl->end,
				    hdl->lors_threads,hdl->lors_timeout,
				    LORS_RETRY_UNTIL_TIMEOUT);
                if(ret != LORS_SUCCESS) {
	                lorsSetFree(set,LORS_FREE_MAPPINGS);
		        errno=EIO;
		        return -1;
                }
	}

	ret = lorsAppendSet(hdl->ex, set);
        if(ret != LORS_SUCCESS) {
	        lorsSetFree(set, LORS_FREE_MAPPINGS);
                errno=EIO;
                return -1;
        }
	
        lorsSetFree(set,0);

	hdl->begin += hdl->b_pos;

	hdl->end = 0;

	ret = lorsFileSerialize(hdl->ex, hdl->filename, 0, 0);
	
        if(ret != LORS_SUCCESS) {
	        perror("file serialize");
	}

	hdl->dirty_buffer = 0;
	return 0;
}

static ssize_t
ibp_write(void *handle, const void *buffer, size_t size)
{
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)handle;

	pthread_testcancel();
	pthread_mutex_lock(&xio_lock);
	pthread_testcancel();
	if(size > hdl->lors_blocksize)
		size = hdl->lors_blocksize;

	if(!hdl || !buffer || hdl->mode == O_RDONLY || !hdl->buffer) {
		errno = EINVAL;
		pthread_mutex_unlock(&xio_lock);
		return -1;
	}

	hdl->dirty_buffer = 1;

	if(hdl->b_pos + size >= hdl->lors_blocksize) {
		if(ibp_flush(handle)) {
			errno = EIO;
			pthread_mutex_unlock(&xio_lock);
			return -1;
		}
		hdl->b_pos = 0;
	}
		
	memcpy((char *)hdl->buffer + hdl->b_pos, buffer, size);
	hdl->b_pos += size;
	if(hdl->end < hdl->b_pos)
		hdl->end = hdl->b_pos;
	
	pthread_mutex_unlock(&xio_lock);
	return size;
}

static ssize_t
ibp_read(void *handle, void *buffer, size_t size)
{
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)handle;
	int ret;
	int msize=size;
	LorsSet *set;

	pthread_testcancel();
	pthread_mutex_lock(&xio_lock);
	pthread_testcancel();
	
	if(hdl->mode == O_WRONLY) {
		errno = EINVAL;
		pthread_mutex_unlock(&xio_lock);
		return -1;
	}

	if(size > hdl->lors_blocksize - hdl->b_pos)
		msize = hdl->lors_blocksize - hdl->b_pos;

	if(hdl->b_pos < hdl->lors_blocksize && !hdl->fill_buffer) {
                memcpy(buffer, hdl->buffer+hdl->b_pos, msize);
                hdl->b_pos += msize;
		pthread_mutex_unlock(&xio_lock);
                return msize;
        }
	
	hdl->fill_buffer = 0;
	
	ret = lorsQuery(hdl->ex, &set, hdl->begin + hdl->b_pos, 
			hdl->lors_blocksize, 0);
	if(ret != LORS_SUCCESS) {
		pthread_mutex_unlock(&xio_lock);
		return 0;
	}

	ret = lorsSetLoad(set, hdl->buffer, hdl->begin + hdl->b_pos, 
			hdl->lors_blocksize, hdl->lors_blocksize,
                              NULL, hdl->lors_threads, hdl->lors_timeout, 0);

	lorsSetFree(set, 0);
	
	if(ret < 0) {
		errno = EIO;
		pthread_mutex_unlock(&xio_lock);
		return -1;
	}

	hdl->begin += hdl->b_pos;

	if(size > ret)
		size = ret;

	hdl->b_pos = size;
	memcpy(buffer, hdl->buffer, size);
	pthread_mutex_unlock(&xio_lock);
	return size;
}

static off_t
ibp_lseek(void *handle, off_t offs, int mode)
{	
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)handle;

	pthread_mutex_lock(&xio_lock);
	pthread_testcancel();
	if(mode == SEEK_SET) {
		if(offs - hdl->begin > hdl->lors_blocksize || 
				offs < hdl->begin) {
			if(ibp_flush(handle)) {
				errno = EIO;
				pthread_mutex_unlock(&xio_lock);
				return -1;
			}
			hdl->fill_buffer = 1;
			hdl->begin = offs;
			hdl->b_pos = 0;
		} else {
			hdl->b_pos = offs - hdl->begin;
		}
	}
	else if(mode == SEEK_CUR) {
		if(hdl->b_pos + offs > hdl->lors_blocksize || 
				hdl->b_pos + offs < 0) {
			if(ibp_flush(handle)) {
				errno = EIO;
				pthread_mutex_unlock(&xio_lock);
				return -1;
			}	
			hdl->fill_buffer = 1;
			hdl->begin = hdl->begin + hdl->b_pos + offs;
			hdl->b_pos = 0;
		} else {
			hdl->b_pos += offs;
		}
	} else if(mode == SEEK_END) {
		if(ibp_flush(handle)) {
			errno = EIO;
			pthread_mutex_unlock(&xio_lock);
			return -1;
		}
		hdl->fill_buffer = 1;
		hdl->begin = hdl->ex->logical_length + offs;
		hdl->b_pos = 0;
	} else {
		errno=EINVAL;
		pthread_mutex_unlock(&xio_lock);
                return(-1);
	}

	pthread_mutex_unlock(&xio_lock);
	return hdl->begin + hdl->b_pos;
}

static int
ibp_close(void *handle) 
{
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)handle;
	int ret;
	
	pthread_mutex_lock(&xio_lock);
	pthread_testcancel();
	if(hdl->mode & O_WRONLY || hdl->mode & O_CREAT) {
		if(hdl->dirty_buffer)
			ibp_flush(handle);
		else {
			ret = lorsFileSerialize(hdl->ex, hdl->filename, 0, 0);
		        if(ret != LORS_SUCCESS) {
		                perror("file serialize");
		        }
		}
	}
	if(hdl->buffer)
		free(hdl->buffer);
	ret = lorsExnodeFree(hdl->ex);
	if(ret != LORS_SUCCESS) {
		perror("exnode free");
	}
	if(!hdl) {
		pthread_mutex_unlock(&xio_lock);
		return 0;
	}
	if(hdl->dp)
		lorsFreeDepotPool(hdl->dp);
	pthread_mutex_unlock(&xio_lock);
	free(hdl);
	return 0;
}

static int
ibp_ftruncate(void *stream, off_t length)
{
	struct xio_ibp_handle_t* hdl = (struct xio_ibp_handle_t*)stream;
	int ret;
	LorsSet *set;

	pthread_mutex_lock(&xio_lock);
	pthread_testcancel();
	ibp_flush(stream);
	hdl->b_pos = 0;
	if(length == hdl->ex->logical_length) {
		pthread_mutex_unlock(&xio_lock);
		return 0;
	}
	
	ret = lorsQuery(hdl->ex, &set, length, 
			hdl->ex->logical_length-length, 0);
        if(ret != LORS_SUCCESS) {
		pthread_mutex_unlock(&xio_lock);
	        errno = EINVAL;
	        return -1;
	}
	ret = lorsSetTrim(set, length, hdl->ex->logical_length-length, 
			1, 20, LORS_TRIM_ALL);
	lorsSetFree(set, 0);
	if(ret != LORS_SUCCESS) {
		pthread_mutex_unlock(&xio_lock);
		errno = EIO;
		return -1;
	}
	pthread_mutex_unlock(&xio_lock);
	return 0;
}

static char *
ibp_lorstoname(char *file_name)
{
	char * uri = file_name;
	if(strncmp(uri, IBP_URI, strlen(IBP_URI)) != 0) {
		return strdup(uri);
	}
	uri += strlen(IBP_URI);
	uri = (char *)(strchr(uri, '/')+1);
	if(!strchr(uri, '?')) {
		return strdup(uri);
	}
	return strndup(uri, (int)(strchr(uri, '?')-uri));
}

static int
ibp_stat(const char *file_name, struct stat *buf)
{
	LorsExnode *exnode;
	int ret;
	char *fn;
	
	fn = ibp_lorstoname((char *)file_name);
	ret = lorsFileDeserialize(&exnode, fn, NULL);
	if(ret!=0) {
                errno=EACCES;
                return -1;
        }
	if(stat(fn, buf) == -1) {
		free(fn);
		return -1;
	}
  	buf->st_ino=-1;
        buf->st_dev=-1;
        buf->st_size=exnode->logical_length;
        lorsExnodeFree(exnode);
	free(fn);
	return 0;
}

static int
ibp_lstat(const char *file_name, struct stat *buf)
{
	char *fn = ibp_lorstoname((char *)file_name);
        if(lstat(fn,buf) == -1) {
		free(fn);
                return -1;
        }
	free(fn);
	return 0;
}

static int
ibp_fstat(void *stream, struct stat *buf)
{
	struct xio_ibp_handle_t *hdl = (struct xio_ibp_handle_t *)stream;

	return ibp_stat(hdl->filename,buf);
}

#endif

static int
io_init()
{
        int i;

        for(i = 0; i < 256; i++)
                _handles[i] = NULL;
	
        xio_initialized = 1;
	pthread_mutex_init(&xio_lock, NULL);
        return 0;
}

int
xio_open(const char *pathname, int flags, ...)
{
	int i;
	int ret_fd;
	int mode=0;

	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
                return -1;
	}
	
	pthread_mutex_lock(&xio_lock);
	// Find free IO handle
        i=3; // skip stdin, stdout, stderr
        while(_handles[i] != NULL && i < 256) i++;
        ret_fd = i;
	if(i == 256) {
		// no free io handle
		pthread_mutex_unlock(&xio_lock);
		errno = EIO;
		return -1;
	}
	
        _handles[ret_fd] = (struct xio_handle_t*)calloc(1,
                                    sizeof(struct xio_handle_t));
	pthread_mutex_unlock(&xio_lock);
	if(!_handles[ret_fd]) {
		// not enough free memory
		return -1;
	}

	if(flags & O_CREAT) {
 		va_list arg;
	       	va_start (arg, flags);
		mode = va_arg (arg, int);
		va_end (arg);
	}

#ifdef HAVE_IBP
	if(strncmp(pathname, IBP_URI, strlen(IBP_URI)) == 0) {
		// IBP uri
		
		_handles[ret_fd]->xio_read_v = &ibp_read;
		_handles[ret_fd]->xio_write_v = &ibp_write;
		_handles[ret_fd]->xio_lseek_v = &ibp_lseek;
		_handles[ret_fd]->xio_close_v = &ibp_close;
		_handles[ret_fd]->xio_ftruncate_v = &ibp_ftruncate;
		_handles[ret_fd]->xio_fstat_v = &ibp_fstat;
		
		_handles[ret_fd]->data = (void *)ibp_open(pathname, flags, mode);
		if(!_handles[ret_fd]->data) {
	                free(_handles[ret_fd]);
	                _handles[ret_fd] = NULL;
			errno = EIO;
	                return -1;
	        }
		return ret_fd;
	} 
#endif
	
	// fallback to unix io routine
	
	_handles[ret_fd]->xio_read_v = (ssize_t(*)(void *, void *, size_t))&read;
	_handles[ret_fd]->xio_write_v = 
		(ssize_t(*)(void *, const void *, size_t))&write;
	_handles[ret_fd]->xio_lseek_v = (off_t(*)(void *,off_t,int))&lseek;
	_handles[ret_fd]->xio_close_v = (int (*)(void *))&close;
	_handles[ret_fd]->xio_ftruncate_v = (int (*)(void *, off_t))&ftruncate;
	_handles[ret_fd]->xio_fstat_v = (int (*)(void *, struct stat *))&fstat;
	
	_handles[ret_fd]->data = (void *)open(pathname, flags, mode);
	
	if(!_handles[ret_fd]->data) {
		free(_handles[ret_fd]);
		_handles[ret_fd] = NULL;
		errno = EIO;
		return -1;
	}
	
	return ret_fd;
}

ssize_t
xio_read(int fd, void *buf, size_t count)
{
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
                return -1;
	}
	if(!_handles[fd])
		return read(fd, buf, count);
	return _handles[fd]->xio_read_v(_handles[fd]->data, buf, count);
}

ssize_t
xio_write(int fd, const void *buf, size_t count)
{
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
                return -1;
	}
	
	if(!_handles[fd])
		return write(fd, buf, count);
	return _handles[fd]->xio_write_v(_handles[fd]->data, buf, count);
}

int
xio_ftruncate(int fd, off_t length)
{
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
		return -1;
	}
	if(!_handles[fd])
		return ftruncate(fd, length);
	return _handles[fd]->xio_ftruncate_v(_handles[fd]->data, length);
}

off_t
xio_lseek(int fd, off_t offset, int whence)
{
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
                return -1;
	}
	
	if(!_handles[fd])
		return lseek(fd, offset, whence);

	return _handles[fd]->xio_lseek_v(_handles[fd]->data, offset, whence);
}

int
xio_close(int fd)
{
	int ret;
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
		return -1;
	}
	if(fd == -1) {
		errno = EINVAL;
		return -1;
	}
	
	if(!_handles[fd])
		return close(fd);
	ret = _handles[fd]->xio_close_v(_handles[fd]->data);	
	free(_handles[fd]);
	_handles[fd]=NULL;
	return ret;
}

int
xio_stat(const char *file_name, struct stat *buf)
{
	if(!xio_initialized && io_init() != 0) {
		errno = EIO;
                return -1;
	}

#ifdef HAVE_IBP
	if(strncmp(file_name, IBP_URI, strlen(IBP_URI)) == 0) {
		return ibp_stat(file_name, buf);
	}
#endif
	return stat(file_name, buf);
}

int
xio_lstat(const char *file_name, struct stat *buf)
{
        if(!xio_initialized && io_init() != 0) {
                errno = EIO;
                return -1;
        }

#ifdef HAVE_IBP
	if(strncmp(file_name, IBP_URI, strlen(IBP_URI)) == 0) {
		return ibp_lstat(file_name, buf);
        }
#endif
        return lstat(file_name, buf);
}

int
xio_rename(const char *oldpath, const char *newpath)
{
#ifdef HAVE_IBP
	char *old, *old_p;
	char *newp;
	int ret;
	
	if(strncmp(IBP_URI, oldpath, strlen(IBP_URI)) == 0) {
		old_p = old = strdup(oldpath);
		old = strchr(old+strlen(IBP_URI),'/')+1;
		if(strchr(old, '?')) {
			*(strchr(old, '?')) = 0;
		} 
			
		newp = (char*)malloc(strlen(old)+1+4);
		snprintf(newp, strlen(old)+1+4, "%s%s", old, &newpath[strlen(newpath)-4]);
		ret = rename(old, newp);
		free(old_p);
		return ret;
	}
#endif
	return rename(oldpath, newpath);
}

int
xio_fstat(int fd, struct stat *buf)
{
        if(!xio_initialized && io_init() != 0) {
                errno = EIO;
                return -1;
        }

        return _handles[fd]->xio_fstat_v(_handles[fd]->data, buf);
}
