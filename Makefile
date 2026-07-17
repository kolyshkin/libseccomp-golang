.PHONY: all
all: check

.PHONY: check
check: lint build test

# Previous bugs have made the tests freeze until the timeout. Golang default
# timeout for tests is 10 minutes, which is too long, considering current tests
# can be executed in less than 1 second. Reduce the timeout, so problems can
# be noticed earlier in the CI.
TEST_TIMEOUT=10s

.PHONY: test
test:
	go test -v -timeout $(TEST_TIMEOUT)

.PHONY: lint
lint:
	golangci-lint run

.PHONY: build
build:
	go build
