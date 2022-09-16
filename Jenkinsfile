pipeline
{
	agent any

	environment
	{
		BLZ3_HOME="$WORKSPACE"
		LD_LIBRARY_PATH="$BLZ3_HOME/lib"

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
				xunit([GoogleTest(pattern: 'src/*/test_*.xml', stopProcessingIfError: false)])
			}
		}

		stage ('Unittests')
		{
			stages
			{
				stage ('short tests')
				{
					steps
					{
						echo "Quick unit tests"
						sh '''
						cd src/unittest
						make -j `nproc` valgrind
						'''
					}
				}
				stage ('long tests')
				{
					when
					{
						anyOf
						{
							branch '*/develop';
							branch '*/master'
						}
					}
					steps
					{
						echo "Slow unit tests"
/*
						sh '''
						cd src/unittest
						make -j `nproc` -f Makefile.longtest valgrind
						'''
*/
					}
				}
				stage ('Reporting')
				{
					steps
					{
						xunit checksName: '', tools: [
							CppUnit(excludesPattern: '', pattern: 'src/*/*test-results.xml', stopProcessingIfError: false),
							Valgrind(excludesPattern: '', pattern: 'src/*/valgrind-*.xml', stopProcessingIfError: false)]
					}
				}
			}
		}

		stage ('Doxygen')
		{
			steps
			{
				sh 'doxygen'
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
		success
		{
			archiveArtifacts artifacts:: 'doc/*.qch', followSymlinks: false
		}
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'blz3-dev@morknet.de'])
		}
	}
}
qmake CONFIG+=debug CONFIG+=gcov -r