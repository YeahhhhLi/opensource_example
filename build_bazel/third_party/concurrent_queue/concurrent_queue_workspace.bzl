"""Loads the queue library"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def repo():
    http_archive(
        name = "concurrent_queue",
        build_file = clean_dep("//third_party/concurren_tqueue:concurrent_queue.BUILD"),
        sha256 = "54d2b038bf018d502fff6d5a863bb85e23ae720730cda6ca9a53f81aab432c3f",
        strip_prefix = "concurrentqueue-1.1.0",
        urls = [
            "https://github.com/YeahhhhLi/concurrentqueue/archive/refs/tags/v1.1.0.tar.gz",
        ],
    )
    #native.new_local_repository(
    #    name = "eigen",
    #    build_file = clean_dep("//third_party/eigen3:eigen.BUILD"),
    #    path = "/usr/include/eigen3",
    #)