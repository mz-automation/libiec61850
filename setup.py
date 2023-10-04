import os
import shutil
import subprocess
import sys
import sysconfig

from setuptools import Extension, find_packages, setup
from setuptools.command.build_ext import build_ext
from setuptools.command.build_py import build_py as _build_py


class CMakeExtension(Extension):
    def __init__(self, name):
        Extension.__init__(self, name, sources=["."])
        self.sourcedir = os.path.abspath(".")


class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: "
                + ", ".join(e.name for e in self.extensions)
            )

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cfg = "Debug" if self.debug else "Release"

        cmake_args = [
            "-DPYTHON_EXECUTABLE=" + sys.executable,
            "-DPYTHON_INCLUDE_DIRS=" + sysconfig.get_paths()["include"],
            "-DBUILD_PYTHON_BINDINGS=ON",
            "-DCMAKE_BUILD_TYPE=" + cfg,
        ]

        build_args = ["--config", cfg]
        env = os.environ.copy()
        env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get("CXXFLAGS", ""), self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        if not os.path.exists(extdir):
            os.makedirs(extdir)

        subprocess.check_call(["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=self.build_temp)

        source_dir = os.path.join(self.build_temp, "pyiec61850")
        for file in os.listdir(source_dir):
            if file == "iec61850.py" or "_iec61850." in file:
                shutil.copy(os.path.join(source_dir, file), extdir)


class build_py(_build_py):
    # See github.com/yanqd0/swig-python-demo
    # Run first build_ext, as Swig will generate new python files that will have to be detected and included
    def run(self):
        self.run_command("build_ext")
        return super().run()


long_description = """
============
pyiec61850
============

Compiled, packaged python bindings of https://github.com/mz-automation/libiec61850.

Wheels are built with the CI of the fork https://github.com/nrontsis/libiec61850.

License same as mz-automation/libiec61850.

Use at your own risk.
"""

setup(
    name="pyiec61850",
    version="1.5.2a1",
    packages=find_packages(),
    description="Python bindings of https://github.com/mz-automation/libiec61850.",
    long_description=long_description,
    ext_modules=[CMakeExtension("pyiec61850/_iec61850")],
    cmdclass={"build_py": build_py, "build_ext": CMakeBuild},
    zip_safe=False,
    install_requires=[],
)
