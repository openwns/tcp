import os
Import('env')
srcFiles,headers,pyconfigs = SConscript('config/libfiles.py')

if len(srcFiles) != 0:
    lib = env.SharedLibrary('tcp', srcFiles)
    env.Install(os.path.join(env.installDir, 'lib'), lib )

