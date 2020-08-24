#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "const.h"

Test(basecode_tests_suite, validargs_help_test) {
    int argc = 2;
    char *argv[] = {"bin/transplant", "-h", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    int opt = global_options;
    int flag = 0x1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert_eq(opt & flag, flag, "Correct bit (0x1) not set for -h. Got: %x", opt);
}

Test(basecode_tests_suite, validargs_serialize_test) {
    int argc = 2;
    char *argv[] = {"bin/transplant", "-s", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    int opt = global_options;
    int flag = 0x2;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
		 ret, exp_ret);
    cr_assert(opt & flag, "Compress mode bit wasn't set. Got: %x", opt);
}
Test(basecode_tests_suite, validargs_serialize_test2) {
    int argc = 3;
    char *argv[] = {"bin/transplant", "-s", "-p",NULL};
    int ret = validargs(argc, argv);
    int exp_ret = -1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
}

Test(basecode_tests_suite, validargs_error_test) {
    int argc = 3;
    char *argv[] = {"bin/transplant", "-s", "-c", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = -1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
		 ret, exp_ret);
}

Test(basecode_tests_suite, validargs_deserialize_test1) {
    int argc = 5;
    char *argv[] = {"bin/transplant", "-d", "-c", "abc","-p",NULL};
    int ret = validargs(argc, argv);
    int exp_ret = -1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
}

Test(basecode_tests_suite, validargs_deserialize_test2) {
    int argc = 4;
    char *argv[] = {"bin/transplant", "-d", "-p", "abc",NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    int opt = global_options;
    int flag = 0x4;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
    cr_assert(opt & flag, "Compress mode bit wasn't set. Got: %x", opt);
}

Test(basecode_tests_suite, validargs_deserialize_test3) {
    int argc = 3;
    char *argv[] = {"bin/transplant", "-d", "-c", NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    int opt = global_options;
    int flag = 0xc;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
    cr_assert(opt & flag, "Compress mode bit wasn't set. Got: %x", opt);
}

Test(basecode_tests_suite, validargs_deserialize_test4) {
    int argc = 5;
    char *argv[] = {"bin/transplant", "-d", "-p", "abc","-c",NULL};
    int ret = validargs(argc, argv);
    int exp_ret = 0;
    int opt = global_options;
    int flag = 0xc;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
    cr_assert(opt & flag, "Compress mode bit wasn't set. Got: %x", opt);
}

Test(basecode_tests_suite, validargs_deserialize_test5) {
    int argc = 4;
    char *argv[] = {"bin/transplant", "-d", "-c","-p",NULL};
    int ret = validargs(argc, argv);
    int exp_ret = -1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
}
Test(basecode_tests_suite, validargs_deserialize_test6) {
    int argc = 5;
    char *argv[] = {"bin/transplant", "-d", "-p","-c","abc"};
    int ret = validargs(argc, argv);
    int exp_ret = -1;
    cr_assert_eq(ret, exp_ret, "Invalid return for validargs.  Got: %d | Expected: %d",
         ret, exp_ret);
}
