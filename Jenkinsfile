pipeline {
    agent any
    environment {
        dockerImage = ''
        currentStage = ''
        WORKSPACE = pwd()
        entryPoint = '-it --rm -v "${WORKSPACE}":/root/workspace'
    }
    stages {
        stage('Cleanup and Checkout') {
            steps {
                echo "Branch_name:${env.BRANCH_NAME}"
                cleanWs()
                checkout scm: [$class: 'GitSCM', branches: [[name: "${env.BRANCH_NAME}"]], userRemoteConfigs: [[credentialsId: 'Yorisoft', url: 'https://github.com/Yorisoft/pokedex_miyoo']]]
            }
        }
		
		stage('print working directory') {
            steps {
                script {
                    dockerImage.inside("${entryPoint}") {
                        sh '''
                            ls -al
                        '''
                    }
                }
            }
        }
		
        /*stage('Build union-trimui-toolchain') {
            steps {
                script {
                    sh 'make shell'
                    dockerImage = docker.image('miyoomini-toolchain-pokedex')
                }
            }
        }
		
		
		
        stage('Build SDL2') {
            steps {
                script {
                    dockerImage.inside("${entryPoint}") {
                        sh '''
                            cd /root/workspace
                            ./mksdl2.sh
                        '''
                    }
                }
            }
        }
        stage('Build Pokedex') {
            steps {
                script {
                    dockerImage.inside("${entryPoint}") {
                        sh '''
                            cd /root/workspace/pokedex
                            mkdir -p build
                            cd build
                            cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake
                            make
                        '''
                    }
                }
            }
        }
        stage('Copy Build Files') {
            steps {
                script {
                    dockerImage.inside("${entryPoint}") {
                        sh '''
                            cd /root/workspace/pokedex
                            rsync -av build/Pokedex build/DownloadIconsbuild/DownloadSprites build/DownloadAnimatedSprites .
                        '''
                    }
                }
            }
        }*/
        stage('Cleanup') {
            steps {
                cleanWs()
                sh 'docker system prune -a -f'
            }
        }
    }
    post {
        always {
            archiveArtifacts allowEmptyArchive: true, followSymlinks: false, artifacts: '**/*.png, **/*.gif', excludes: '**/!(Source/union-miyoomini-toolchain/workspace/pokedex/res/types|Source/union-miyoomini-toolchain/workspace/pokedex/res/icons/icon|Source/windows-visual-studio/Pokedex/res/types|Source/windows-visual-studio/Pokedex/res/icons/icon|App/Pokedex/res/types|App/Pokedex/res/icons/icon)/*.png, **/!(Source/union-miyoomini-toolchain/workspace/pokedex/res/types|Source/union-miyoomini-toolchain/workspace/pokedex/res/icons/icon|Source/windows-visual-studio/Pokedex/res/types|Source/windows-visual-studio/Pokedex/res/icons/icon|App/Pokedex/res/types|App/Pokedex/res/icons/icon)/*.gif, Source/union-miyoomini-toolchain/workspace/pokedex/build/**, Source/windows-visual-studio/Pokedex/Debug/**, Source/windows-visual-studio/Pokedex/.vs/**, Source/windows-visual-studio/Pokedex/x64/**'
        }
    }
}
