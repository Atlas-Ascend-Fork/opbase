# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Dependency installation must propagate failed command statuses."""

import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[2] / "install_deps.sh"
BASH = shutil.which("bash")


@unittest.skipUnless(BASH, "Bash is required")
class InstallDepsStatusTest(unittest.TestCase):
    def run_driver(self, command):
        text = SCRIPT.read_text(encoding="utf-8")
        function = re.search(r"^run_command\(\) \{.*?^\}", text, re.M | re.S).group()
        env = os.environ.copy()
        env.pop("BASH_ENV", None)
        with tempfile.TemporaryDirectory(dir=os.getenv("OPBASE_TEST_TMPDIR")) as directory:
            driver = Path(directory) / "driver.sh"
            with driver.open("w", encoding="utf-8", newline="\n") as stream:
                stream.write(
                    "set -euo pipefail\n" + function + "\n" + command
                    + "\nprintf 'AFTER_COMMAND\\n'\n"
                )
            return subprocess.run(
                [BASH, driver.as_posix()], env=env, capture_output=True,
                text=True, timeout=10, check=False,
            )

    def test_success_continues(self):
        result = self.run_driver("run_command bash -c 'exit 0'")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("AFTER_COMMAND", result.stdout)

    def test_exit_one_is_preserved(self):
        result = self.run_driver("run_command bash -c 'exit 1'")
        self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
        self.assertIn("Exit code: 1", result.stdout)
        self.assertNotIn("AFTER_COMMAND", result.stdout)

    def test_failure_keeps_arguments_and_diagnostics(self):
        result = self.run_driver(
            "run_command bash -c 'printf \"%s\\n\" \"$1\"; exit 7' _ 'two words'"
        )
        self.assertEqual(result.returncode, 7, result.stdout + result.stderr)
        self.assertIn("two words", result.stdout)
        self.assertIn("Exit code: 7", result.stdout)
        self.assertNotIn("AFTER_COMMAND", result.stdout)

    def test_missing_command_is_preserved(self):
        result = self.run_driver("run_command opbase_missing_command_status_test")
        self.assertEqual(result.returncode, 127, result.stdout + result.stderr)
        self.assertIn("Exit code: 127", result.stdout)
        self.assertNotIn("AFTER_COMMAND", result.stdout)

    def test_complete_cli_reports_package_manager_failure(self):
        # The controlled Darwin branch never invokes real package installation.
        # Python satisfies its check; GCC then requests brew, which fails.
        env = os.environ.copy()
        with tempfile.TemporaryDirectory(dir=os.getenv("OPBASE_TEST_TMPDIR")) as directory:
            root = Path(directory)
            startup = root / "startup.sh"
            source = root / "install_deps.sh"
            with startup.open("w", encoding="utf-8", newline="\n") as stream:
                stream.write(
                    "uname() { printf 'Darwin\\n'; }\n"
                    "python3() { printf 'Python 3.13.9\\n'; }\n"
                    "gcc() { printf 'gcc (test) 0.0.0\\n'; }\n"
                    "xcode-select() { return 0; }\n"
                    "brew() { printf 'controlled package manager failure\\n' >&2; return 7; }\n"
                )
            with source.open("w", encoding="utf-8", newline="\n") as stream:
                stream.write(SCRIPT.read_text(encoding="utf-8"))
            env["BASH_ENV"] = startup.as_posix()
            result = subprocess.run(
                [BASH, source.as_posix()], env=env, capture_output=True,
                text=True, timeout=10, check=False,
            )
        self.assertIn("controlled package manager failure", result.stdout)
        self.assertEqual(result.returncode, 7, result.stdout + result.stderr)
        self.assertIn("Exit code: 7", result.stdout)
        self.assertNotIn("All dependencies installed successfully", result.stdout)


if __name__ == "__main__":
    unittest.main()
