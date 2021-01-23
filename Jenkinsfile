library(identifier: 'JenkinsPipelineUtilities@master', retriever: modernSCM([$class: 'GitSCMSource', credentialsId: '', remote: 'https://github.com/timrademaker/JenkinsPipelineUtilities.git']))
 
pipeline {
    parameters {
        // Project
        string(defaultValue: params.CONFIGURATION ? params.CONFIGURATION : 'Development', description: 'The build configuration for this project. Options: Development, Shipping.', name: 'CONFIGURATION', trim: true)
        string(defaultValue: params.TARGET_PLATFORM ? params.TARGET_PLATFORM : 'Win64', description: 'The target platform to build the project for. Options: Win32, Win64, HoloLens, Mac, XboxOne, PS4, IOS, Android, Linux, LinuxAArch64, AllDesktop, TVOS, Switch, Quail, Lumin.', name: 'TARGET_PLATFORM', trim: true)
 
        // Shipping
        booleanParam(defaultValue: params.SHOULD_DEPLOY ? params.SHOULD_DEPLOY : false, description: 'True if the build should be deployed.', name: 'SHOULD_DEPLOY')
        booleanParam(defaultValue: params.DEPLOY_UNSTABLE_BUILD ? params.DEPLOY_UNSTABLE_BUILD : false, description: 'True if the build should be deployed even if it is marked as unstable.', name: 'DEPLOY_UNSTABLE_BUILD')
        credentials(credentialType: 'org.jenkinsci.plugins.plaincredentials.impl.StringCredentialsImpl', defaultValue: params.ITCH_API_KEY ? params.ITCH_API_KEY : '', description: 'The itch.io API key to use when shipping', name: 'ITCH_API_KEY')
        string(defaultValue: params.ITCH_CHANNEL ? params.ITCH_CHANNEL : '', description: 'The channel to which this game should be deployed.', name: 'ITCH_CHANNEL', trim: true)
        string(defaultValue: params.IGNORED_FILES ? params.IGNORED_FILES : '', description: 'A list of files to ignore when shipping, separated with a \';\'.', name: 'IGNORED_FILES', trim: true)
        booleanParam(defaultValue: params.DRY_RUN ? params.DRY_RUN : false, description: 'True if this build is a test build (i.e. should not actually be deployed).', name: 'DRY_RUN')
    }
    
    agent any;
    
    options {
        timeout(time: 45, unit: 'MINUTES')
    }
    
    environment {
        // BitBucket
        GIT_CREDENTIALS = 'Tim_BitBucket'                                       // The ID of the git credentials to use
        GIT_REPO_URL = 'https://bitbucket.org/timrademaker/sfas_2021_unreal'    // The url of the git repo to build

        // Project
        PROJECT_NAME = 'Mazeum'                                     // The name of the project file (without '.uproject')
        PROJECT_DIRECTORY = '.'                                     // Project directory, relative to the workspace root
        BUILD_OUT_DIR = "${env.WORKSPACE}/out/build"                // The folder in which to output the build

        // itch.io
        ITCH_USERNAME = 'timrademaker'                              // The username of the itch.io account associated with the API key
        ITCH_GAME_NAME = 'mazeum'                                   // The name of the game to ship to itch.io (i.e. the last part of the url)
        ITCH_BUTLER_EXE = 'D:/Programs/itch.io_Butler/butler.exe'   // The path to itch.io's butler executable

        // Unreal Engine base directory
        UE_DIR = 'C:/Program Files/Epic Games/UE_4.25';
    }
    
    stages {
        stage('Setup') {
            steps {
                script {
                    git credentialsId: env.GIT_CREDENTIALS, url: env.GIT_REPO_URL

                    ue4.init(env.UE_DIR, params.TARGET_PLATFORM, params.CONFIGURATION);

                    if(params.SHOULD_DEPLOY) {
                        itch.init(env.ITCH_BUTLER_EXE, params.ITCH_API_KEY, env.ITCH_USERNAME);
                    }
                }
            }
        }
        stage('Build') {
            when {
                expression {
                    fileExists("${env.WORKSPACE}/${env.PROJECT_DIRECTORY}/Source/${env.PROJECT_NAME}.Target.cs")
                }
            }
            
            steps {
                script {
                    ue4.build("${env.WORKSPACE}/${env.PROJECT_DIRECTORY}", env.PROJECT_NAME);
                }
            }
        }
        stage('Package') {
            steps {
                script {
                    ue4.packageProject("${env.WORKSPACE}/${env.PROJECT_DIRECTORY}", env.PROJECT_NAME, env.BUILD_OUT_DIR);
                }
            }
        }
        stage('Deploy') {
            when {
                expression {
                    params.SHOULD_DEPLOY &&
                    (currentBuild.result != 'UNSTABLE' || params.DEPLOY_UNSTABLE_BUILD)
                }
            }
            
            steps {
                script {
                    platformName = params.TARGET_PLATFORM;
                    if(params.TARGET_PLATFORM.toLowerCase() in ['win64', 'win32']) {
                        platformName = 'Windows';
                    }
                    
                    contentRoot = "${env.BUILD_OUT_DIR}/${platformName}NoEditor";
                    itch.push(contentRoot, env.ITCH_GAME_NAME, params.ITCH_CHANNEL, params.DRY_RUN, params.IGNORED_FILES.split(';') as List<String>);
                }
            }
            
        }
    }
    
    post {
        always {
            script {
                log.parse();
            }
        }
 
        cleanup {
            script {
                file.delete("${env.WORKSPACE}/out");
                file.delete("${env.WORKSPACE}/temp");
            }
        }
    }
}