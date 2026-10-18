import re
from pathlib import Path

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy
from conan.tools.scm import Git


class RpMidwareConan(ConanFile):
    name = "rp-midware"
    package_type = "static-library"
    settings = "os", "arch", "compiler", "build_type"

    def set_version(self):
        if self.version:
            return
        root = Path(self.recipe_folder).parent
        git = Git(self, folder=str(root))
        version_file = root / "VERSION"
        base = version_file.read_text(encoding="utf-8").strip()
        count = git.run("rev-list --count HEAD")
        commit = git.run("rev-parse --short HEAD")
        tags = git.run("tag --points-at HEAD")
        release = any(re.match(r"^(release|main)/", tag) for tag in tags.splitlines())
        dirty = git.is_dirty()
        development = f"-dev.{count}" if dirty or not release else ""
        suffix = ".dirty" if dirty else ""
        self.version = f"{base}{development}+{commit}{suffix}"

    def export_sources(self):
        # Paths are relative to the repository root, not platform/.
        root = Path(self.recipe_folder).parent
        for name in ("CMakeLists.txt", "VERSION", "LICENSE"):
            copy(self, name, src=str(root), dst=self.export_sources_folder)
        copy(
            self, "*", src=str(root / "midware"),
            dst=str(Path(self.export_sources_folder) / "midware"),
            excludes=("protocol/nanopb/*", "*/build/*", "*.pyc", "*.o", "*.a"),
        )

    def requirements(self):
        self.requires(
            "nanopb/0.4.9.2", options={"shared": False},
            transitive_headers=True, transitive_libs=True,
        )

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, "17")

    def layout(self):
        self.folders.root = ".."
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        toolchain = CMakeToolchain(self)
        toolchain.user_presets_path = False
        toolchain.variables["CMAKE_INSTALL_LIBDIR"] = "lib"
        if str(self.settings.os) == "baremetal":
            toolchain.variables["CMAKE_TRY_COMPILE_TARGET_TYPE"] = "STATIC_LIBRARY"
        toolchain.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        CMake(self).install()
        copy(self, "LICENSE", src=self.source_folder,
             dst=str(Path(self.package_folder) / "licenses"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "rp-midware")
        # 声明组件
        protocol = self.cpp_info.components["protocol"]
        protocol.libs = ["midware_protocol"]
        protocol.includedirs = ["include"]
        protocol.requires = ["nanopb::nanopb"]
        protocol.set_property("cmake_target_name", "midware::protocol")
