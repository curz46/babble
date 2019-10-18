1. INTRODUCTION

Babble is a Discord library written in C. It was created due to the void of
approachable, feature-rich libraries which are abundant in languages like Java
and JavaScript.

2. DEPENDENCIES

Babble's dependency, `libuwsc`, depends on `libev`. Ensure this is installed
before attempting to build babble.

3. ROADMAP

[ ] Gateway
	[ ] Authorization
	[ ] Recognize all payload types
[ ] Expose Discord's REST API through simple method calls
	[ ] Channel endpoins
	[ ] Guild endpoints
	[ ] User endpoints
	[ ] Invite endpoints
	[ ] Emoji endpoints
	[ ] Voice endpoints
	[ ] Audit log endpoints
[ ] Caching
	[ ] ???
[ ] ???

4. NOTES

Known issues:

- When running `make vendor`, `libuwsc` may fail to build because libev cannot
  be found. This seems to be resolvable by executing `cmake ..` inside of
  vendor/libuwsc/build and then re-running `make libuwsc`. Need to find a way
  to tell cmake that we're using a cloned libev.
