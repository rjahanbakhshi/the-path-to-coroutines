from conan import ConanFile
from conan.tools.cmake import cmake_layout

class TaskScheduler(ConanFile):
    name = "the-path-to-coroutines"
    version = "1.0"
    license = "Boost Software License, Version 1.0"
    author = "reza.jahanbakhshi@gmail.com"
    url = "https://github.com/rjahanbakhshi/the-path-to-coroutines.git"
    description = "Pro Task scheduler service."
    generators = "CMakeDeps"
    settings = "os", "compiler", "arch", "build_type"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("boost/1.84.0")

    def configure(self):
        self.options["boost"].shared = False
