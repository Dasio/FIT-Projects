.PHONY: all test report

PHPUNITFLAGS=--bootstrap bootstrap.php
LOGFILE=unittest.log.json
REPORTDIR=coverage-report

test:
	phpunit $(PHPUNITFLAGS) -v --debug .

report:
	-phpunit $(PHPUNITFLAGS) --log-json $(LOGFILE) --coverage-html $(REPORTDIR) .
	@echo
	@echo Check $(REPORTDIR)/index.html

clean:
	-rm -rf $(REPORTDIR)/*
	-rm $(LOGFILE)
