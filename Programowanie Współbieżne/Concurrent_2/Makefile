# You may edit this makefile as long as you keep these original 
# target names defined.  You can change the recipes and/or add new targets.

# Not intended for manual invocation.
# Invoked if automatic builds are enabled.
# Analyzes only on those sources that have changed.
# Does not build executables.
autobuild:
	$(GNATMAKE) -gnatc -c -k  -d -P "$(GPRPATH)"

# Clean the root project of all build products.
clean:
	$(GNATCLEAN) -P "$(GPRPATH)"

# Clean root project and all imported projects too.
clean_tree:
	$(GNATCLEAN) -P "$(GPRPATH)" -r

# Check project sources for errors.
# Does not build executables.
analyze:
	$(GNATMAKE) -d  -gnatc -c -k  -P "$(GPRPATH)"

# Build executables for all mains defined by the project.
build:
	$(GNATMAKE) -d -P "$(GPRPATH)"

# Clean, then build executables for all mains defined by the project.
rebuild: clean build

# Compile individual file.
compile_file:
	$(GNATMAKE) -d -ws -c -u -P "$(GPRPATH)" "$(FILE)"

# Analyze individual file (no object code generated).
analyze_file:
	$(GNATMAKE) -d -q -c -gnatc -u -P "$(GPRPATH)" "$(FILE)"
