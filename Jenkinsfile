pipeline {
    agent any
    environment {
        dockerImage = 'miyoomini-toolchain-pokedex' // Set this to your Docker image
        currentStage = ''
		WORKING_DIR = 'Source/union-miyoomini-toolchain'
        entryPoint = '--rm -v /var/lib/jenkins/workspace/Pokedex_Miyoo_jenkins/Source/union-miyoomini-toolchain/workspace:/root/workspace'
    }
    stages {
        stage('Cleanup and Checkout') {
            steps {
                echo "Branch_name:${env.BRANCH_NAME}"
                cleanWs()
                checkout scm: [$class: 'GitSCM', branches: [[name: "${env.BRANCH_NAME}"]], userRemoteConfigs: [[credentialsId: 'Yorisoft', url: 'https://github.com/Yorisoft/pokedex_miyoo']]]
            }
        }			
		
        stage('Build union-trimui-toolchain') {
            steps {
                script {
					try{
						sh """#!/bin/bash
							ls -a
							cd "${env.WORKING_DIR}"
							ls -a
							chmod +x support/setup-toolchain.sh support/setup-env.sh
							make shell
						"""
						dockerImage = docker.image('miyoomini-toolchain-pokedex')
					} catch (e) {
                        echo "Caught exception: ${e}"
                        currentBuild.result = 'FAILURE'
						throw e
                    }
                }
            }
        }
		
        stage('Build SDL2') {
            steps {
                script {
					try {
						dockerImage.inside("--user root ${env.entryPoint}") {							
							sh """#!/bin/bash
								pwd
								ls -al
								cd "${env.WORKING_DIR}/workspace"
								pwd
								ls -al	
								chmod +x mksdl2.sh
								./mksdl2_pipeline.sh
							""" 
						}
					} catch (e) {
                        echo "Caught exception: ${e}"
                        currentBuild.result = 'FAILURE'
						throw e
                    }
                }
            }
        }
		
        stage('Build Pokedex') {
            steps {
                script {
					try {
						dockerImage.inside("--user root ${env.entryPoint}") {							
							sh """#!/bin/bash
								pwd
								ls -a
								cd "${env.WORKING_DIR}/workspace/pokedex"
								pwd
								ls -a
								mkdir -p build
								cd build
								cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake
								make
							"""
						}
					} catch (e) {
                        echo "Caught exception: ${e}"
                        currentBuild.result = 'FAILURE'
						throw e
                    }
                }
            }
        }
		
        stage('Copy Build Files') {
            steps {
                script {
					try {
						docker.image("--user root ${env.dockerImage}").inside("${env.entryPoint}") {
							sh """#!/bin/bash
								cd "${env.WORKING_DIR}/workspace/pokedex"
								rsync -av build/Pokedex build/DownloadIconsbuild/DownloadSprites build/DownloadAnimatedSprites .
							"""
						}
					} catch (e) {
                        echo "Caught exception: ${e}"
                        currentBuild.result = 'FAILURE'
						throw e
                    }
                }
            }
        }
		
        
		
    }
    post {
        always {			
            archiveArtifacts allowEmptyArchive: true, followSymlinks: false, artifacts: '**/*.png, **/*.gif', excludes: '**/!(Source/union-miyoomini-toolchain/workspace/pokedex/res/types|Source/union-miyoomini-toolchain/workspace/pokedex/res/icons/icon|Source/windows-visual-studio/Pokedex/res/types|Source/windows-visual-studio/Pokedex/res/icons/icon|App/Pokedex/res/types|App/Pokedex/res/icons/icon)/*.png, **/!(Source/union-miyoomini-toolchain/workspace/pokedex/res/types|Source/union-miyoomini-toolchain/workspace/pokedex/res/icons/icon|Source/windows-visual-studio/Pokedex/res/types|Source/windows-visual-studio/Pokedex/res/icons/icon|App/Pokedex/res/types|App/Pokedex/res/icons/icon)/*.gif, Source/union-miyoomini-toolchain/workspace/pokedex/build/**, Source/windows-visual-studio/Pokedex/Debug/**, Source/windows-visual-studio/Pokedex/.vs/**, Source/windows-visual-studio/Pokedex/x64/**'
			cleanWs()
			sh 'docker system prune -a -f'
		}
    }
}
