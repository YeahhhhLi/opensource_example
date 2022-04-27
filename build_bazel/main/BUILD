load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

exports_files([
    "CPPLINT.cfg",
])

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
        "//lib:hello-time",
        "@my_concurrentqueue//:concurrentqueue"
    ],
)
