pipeline
{
	agent
	{
		label 'QT'
	}

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
				echo "Blizzard III home: $BLZ3_HOME"
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

		stage ('Build')
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
				publishCppcheck pattern: 'src/cppcheck.xml'
			}
		}

		stage ('SCT tests')
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

		stage ('Unit tests')
		{
			stages
			{
				stage ('Short tests')
				{
					steps
					{
						echo "Short unit tests"
						sh '''
						cd src/unittest
						make -j `nproc` valgrind
						'''
					}
				}
				stage ('Long tests')
				{
					when
					{
						anyOf
						{
							branch 'develop';
							branch 'master'
						}
					}
					steps
					{
						echo "Long unit tests"
						sh '''
						cd src/unittest
						make -j `nproc` -f Makefile.longtest valgrind
						'''
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
				sh 'make documentation'
				publishHTML([
					allowMissing: false,
					alwaysLinkToLastBuild: false,
					keepAll: false,
					reportDir: 'doc',
					reportFiles: 'index.html',
					reportName: 'Blizzard III Doxygen',
					reportTitles: ''])
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
					reportName: 'Blizzard III Coverage Report',
					reportTitles: ''])
			}
		}
	}

	post
	{
		success
		{
			archiveArtifacts artifacts: 'doc/*.qch', followSymlinks: false
		}
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'blz3-dev@morknet.de'])
		}
	}
}
