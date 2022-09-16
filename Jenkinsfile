pipeline
{
	agent any

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
				make -j `nproc`
				make install
				'''
			}
		}

		stage ('cppcheck')
		{
			steps
			{
				sh 'make cppcheck'
			}
		}

		stage ('Coverage')
		{
			steps
			{
				sh 'make lcov'
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