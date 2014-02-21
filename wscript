def options(opt):
    opt.load('compiler_cxx unittest_gtest')

def configure(conf):
    conf.load('compiler_cxx unittest_gtest')
    conf.env.CXXFLAGS += ['-O3', '-Wall', '-std=c++0x']

def build(bld):
    bld.program(source = 'build.cpp', target = 'matsu-build')
    bld.program(source = 'lookup.cpp', target = 'matsu-lookup')
    bld.program(source = 'traverse.cpp', target = 'matsu-traverse')
    bld.program(source = bld.path.ant_glob('*Test.cpp'), target = 'test',
            features = 'testt', lib=['gtest','gtest_main','pthread'])

