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
				sh 'pwd'
			}
		}
	}

	post
	{
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'linux-dev@morknet.de'])
		}
	}
}
qmake CONFIG+=debug CONFIG+=gcov -r