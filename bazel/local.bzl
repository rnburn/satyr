def _repository_impl(ctx):
  ctx.symlink(ctx.os.environ[ctx.attr.root], ctx.name)
  ctx.file("WORKSPACE", "workspace(name = \"{name}\")\n".format(name=ctx.name))
  ctx.symlink(ctx.attr.build_file, "BUILD")

def new_local_env_repository(name, build_file, root):
  rule = repository_rule(
      implementation = _repository_impl,
      environ = [root],
      attrs = {
          "build_file": attr.label(mandatory=True),
          "root": attr.string(mandatory=True)
      })
  rule(name = name, build_file = build_file, root = root)
