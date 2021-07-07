#! /usr/bin/env python
# encoding: utf-8

from TaskGen import feature,taskgen,before,task_gen
import Node,Task,Utils,Build,pproc,Constants
import Options
import shellcmd
shellcmd.subprocess=pproc
from Logs import debug,error
shellcmd.debug=debug
import Task
import re
arg_rx=re.compile(r"(?P<dollar>\$\$)|(?P<subst>\$\{(?P<var>\w+)(?P<code>.*?)\})",re.M)
class command_task(Task.Task):
	color="BLUE"
	def __init__(self,env,generator):
		Task.Task.__init__(self,env,normal=1,generator=generator)
	def __str__(self):
		env=self.env
		src_str=' '.join([a.nice_path(env)for a in self.inputs])
		tgt_str=' '.join([a.nice_path(env)for a in self.outputs])
		if self.outputs:
			sep=' -> '
		else:
			sep=''
		pipeline=shellcmd.Pipeline()
		pipeline.parse(self.generator.command)
		cmd=pipeline.get_abbreviated_command()
		return'command (%s): %s%s%s\n'%(cmd,src_str,sep,tgt_str)
	def _subst_arg(self,arg,direction,namespace):
		def repl(match):
			if match.group('dollar'):
				return"$"
			elif match.group('subst'):
				var=match.group('var')
				code=match.group('code')
				result=eval(var+code,namespace)
				if isinstance(result,Node.Node):
					if var=='TGT':
						return result.bldpath(self.env)
					elif var=='SRC':
						return result.srcpath(self.env)
					else:
						raise ValueError("Bad subst variable %r"%var)
				elif result is self.inputs:
					if len(self.inputs)==1:
						return result[0].srcpath(self.env)
					else:
						raise ValueError("${SRC} requested but have multiple sources; which one?")
				elif result is self.outputs:
					if len(self.outputs)==1:
						return result[0].bldpath(self.env)
					else:
						raise ValueError("${TGT} requested but have multiple targets; which one?")
				else:
					return result
			return None
		return arg_rx.sub(repl,arg)
	def run(self):
		pipeline=shellcmd.Pipeline()
		pipeline.parse(self.generator.command)
		namespace=self.env.get_merged_dict()
		if self.generator.variables is not None:
			namespace.update(self.generator.variables)
		namespace.update(env=self.env,SRC=self.inputs,TGT=self.outputs)
		for cmd in pipeline.pipeline:
			if isinstance(cmd,shellcmd.Command):
				if isinstance(cmd.stdin,basestring):
					cmd.stdin=self._subst_arg(cmd.stdin,'in',namespace)
				if isinstance(cmd.stdout,basestring):
					cmd.stdout=self._subst_arg(cmd.stdout,'out',namespace)
				if isinstance(cmd.stderr,basestring):
					cmd.stderr=self._subst_arg(cmd.stderr,'out',namespace)
				for argI in xrange(len(cmd.argv)):
					cmd.argv[argI]=self._subst_arg(cmd.argv[argI],None,namespace)
				if cmd.env_vars is not None:
					env_vars=dict()
					for name,value in cmd.env_vars.iteritems():
						env_vars[name]=self._subst_arg(value,None,namespace)
					cmd.env_vars=env_vars
			elif isinstance(cmd,shellcmd.Chdir):
				cmd.dir=self._subst_arg(cmd.dir,None,namespace)
		return pipeline.run(verbose=(Options.options.verbose>0))
def init_command(self):
	Utils.def_attrs(self,variables=None)
def apply_command(self):
	self.meths.remove('apply_core')
	task=self.create_task('command')
	setattr(task,"dep_vars",getattr(self,"dep_vars",None))
	inputs=[]
	for src in self.to_list(self.source):
		node=self.path.find_resource(src)
		if node is None:
			raise Utils.WafError("source %s not found"%src)
		inputs.append(node)
	task.set_inputs(inputs)
	task.set_outputs([self.path.find_or_declare(tgt)for tgt in self.to_list(self.target)])
class command_taskgen(task_gen):
	def __init__(self,*k,**kw):
		task_gen.__init__(self,*k,**kw)
		self.features.append('command')

taskgen(init_command)
feature('command')(init_command)
taskgen(apply_command)
feature('command')(apply_command)
before('apply_core')(apply_command)
