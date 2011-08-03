#!/usr/bin/python

APPNAME = 'ranking'
VERSION = '0.1.0'

top = '.'
out = 'build'

def options(opt):
    opt.tool_options('compiler_cxx')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.env.append_unique('CXXFLAGS', ['-O2', '-W', '-Wall'])
    conf.env.append_unique('LINKFLAGS', ['-O2', '-W', '-Wall'])
    conf.env.HPREFIX=conf.env.PREFIX + '/include/ranking'

def build(bld):
    bld.SRCPATH=bld.path.abspath()+'/src'
    bld.recurse('src')
