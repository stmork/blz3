pipeline
{
	agent any

	environment
	{
		OFLAGS='-O3'
		CFLAGS=''
		CXXFLAGS=''
		CPPFLAGS=''
	}

	stages
	{
		stage ('Configure')
		{
			steps
			{
				sh 'make config depend'
			}
		}

		stage ('qmake')
		{
			steps
			{
				sh '''
					cd src
					qmake CONFIG+=debug CONFIG+=gcov -r
					rm */Makefile
				'''
			}
		}

		stage ('build')
		{
			steps
			{
				sh '''
				make -j `nproc` install
				make -j `nproc`
				'''
			}
		}

		stage ('cppcheck')
		{
			steps
			{
				sh 'make cppcheck'
				publishCppcheck pattern: 'cppcheck.xml'
			}
		}

		stage ('SCT-Unit')
		{
			steps
			{
				sh '''
				cd src/OpenGL
				./qrender-sct --gtest_output=xml
				'''
				xunit([GoogleTest(pattern: 'src/*/test-*.xml', stopProcessingIfError: false)])
			}
		}

		stage ('Unittests')
		{
			steps
			{
				sh '''
				cd src/unittest
				make -j `nproc` valgrind
				'''
				xunit checksName: '', tools: [
					CppUnit(excludesPattern: '', pattern: 'src/*/*test-results.xml', stopProcessingIfError: false),
					Valgrind(excludesPattern: '', pattern: 'src/*/valgrind_*.xml', stopProcessingIfError: false)]
			}
		}

		stage ('Coverage')
		{
			steps
			{
				sh 'make lcov'
				publishHTML([
					allowMissing: false,
					alwaysLinkToLastBuild: false,
					keepAll: false,
					reportDir: 'lcov-out',
					reportFiles: 'index.html',
					reportName: '',
					reportTitles: 'Coverage Report Blizzard III'])
			}
		}
	}

	post
	{
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'blz3-dev@morknet.de'])
		}
	}
}
qmake CONFIG+=debug CONFIG+=gcov -r