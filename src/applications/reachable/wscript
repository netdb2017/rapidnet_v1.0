## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('reachable', ['internet-stack'])
    module.source = [
        'reachable.cc',
        ]
    headers = bld.new_task_gen('ns3header')
    headers.module = 'reachable'
    headers.source = [
        'reachable.h',
        'reachable-helper.h',
        ]
