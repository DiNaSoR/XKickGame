
CERTIFY_PATH = certify
GAME_PATH = game

all :
	@cd $(CERTIFY_PATH); $(MAKE)
	@cd $(GAME_PATH); $(MAKE)

certify :
	@cd $(CERTIFY_PATH); $(MAKE)

game :
	@cd $(GAME_PATH); $(MAKE)

clean :
	@cd $(CERTIFY_PATH); $(MAKE) clean
	@cd $(GAME_PATH); $(MAKE) clean

copy :
	@cd $(CERTIFY_PATH); $(MAKE) copy
	@cd $(GAME_PATH); $(MAKE) copy

dir :
	cd /sv;mkdir kicks
	cd kicks;mkdir certify
	mkdir game8000
	mkdir game8001
	mkdir game8002
	mkdir game8003
	mkdir game8004
	mkdir game8005
	mkdir game8006
	mkdir game8007
	mkdir game8008
	mkdir game8009
	cd /sv/arena
	copy config.ini /sv/kicks
