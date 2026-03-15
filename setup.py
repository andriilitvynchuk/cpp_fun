"""
Build and install the start_lib Python package.
The C++ extension is built with CMake when you run:
  pip install .
  pip install -e .
  python setup.py build_ext
"""
import os
import sys
import glob
import shutil
import subprocess
from pathlib import Path

from setuptools import setup, find_packages
from setuptools.command.build_ext import build_ext


class CMakeBuildExt(build_ext):
    """Build the pybind11 extension via CMake, then copy the .so into the package."""

    def run(self):
        # Ensure package layout is built first so self.build_lib exists
        self.run_command("build_py")
        root = Path(__file__).resolve().parent
        build_dir = Path(self.build_temp) / "cmake_build"
        build_dir.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            "-DCMAKE_BUILD_TYPE=Release",
            "-DBUILD_PYTHON=ON",
            "-DBUILD_TESTS=OFF",
            f"-DPython_EXECUTABLE={sys.executable}",
        ]
        subprocess.check_call(
            ["cmake", str(root), "-B", str(build_dir)] + cmake_args,
            cwd=root,
        )
        subprocess.check_call(
            ["cmake", "--build", str(build_dir)],
            cwd=root,
        )

        # Find built extension (_start_lib*.so)
        so_pattern = str(build_dir / "**" / "_start_lib*.so")
        so_files = glob.glob(so_pattern)
        if not so_files:
            so_files = glob.glob(str(build_dir / "**" / "*.so"), recursive=True)
        if not so_files:
            raise RuntimeError("CMake did not produce a .so file")

        dest_dir = Path(self.build_lib) / "start_lib"
        dest_dir.mkdir(parents=True, exist_ok=True)
        source_pkg_dir = root / "start_lib"  # so editable install finds the .so
        for src in so_files:
            name = os.path.basename(src)
            shutil.copy2(src, dest_dir / name)
            shutil.copy2(src, source_pkg_dir / name)


setup(
    name="start_lib",
    version="1.0.0",
    description="Python bindings for start C++ library (Sensor API)",
    packages=find_packages(),
    python_requires=">=3.8",
    cmdclass={"build_ext": CMakeBuildExt},
    # So the built .so is included when installing from built wheel/sdist
    package_data={"start_lib": ["*.so", "*.pyd"]},
    include_package_data=True,
)
