ls src | Where-Object { $_.Extension -eq ".h" -or $_.Extension -eq ".cpp" } | foreach {clang-format -i -style="{BasedOnStyle: mozilla, IndentWidth: 4}" -sort-includes "src/$_"}
