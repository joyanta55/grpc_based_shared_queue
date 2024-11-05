load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")
http_archive(
    name = "rules_foreign_cc",
    sha256 = "a2e6fb56e649c1ee79703e99aa0c9d13c6cc53c8d7a0cbb8797ab2888bbc99a3",
    strip_prefix = "rules_foreign_cc-0.12.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/releases/download/0.12.0/rules_foreign_cc-0.12.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.12.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

load("@bazel_features//:deps.bzl", "bazel_features_deps")

bazel_features_deps()

# For grpc
http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "1ac33c14ae581a2000864a36db3f7305d4823fd7acb27436c7751251023fe619",
    strip_prefix = "grpc-1.67.0",
    url = "https://github.com/grpc/grpc/archive/refs/tags/v1.67.0.zip",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()
# Not mentioned in official docs... mentioned here https://github.com/grpc/grpc/issues/20511
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()

http_archive(
    name = "cjson_remote_lib",
    sha256 = "0c938cdc091fffbe9091d29ffafe17c4c15e184fc0aca8430517c80ac1bc5a97",
    url = "https://github.com/joyanta55/cJSON/archive/refs/heads/master.zip",
    strip_prefix = "cJSON-master",
    build_file_content = "",
)
