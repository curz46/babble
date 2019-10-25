## Babble

### 1. What is it?

Babble is a Discord library written in C. It was created due to the void of
approachable, feature-rich libraries which are abundant in languages like Java
and JavaScript.

### 2. Where is it? (roadmap)

- [ ] Gateway
	- [ ] Authorization
	- [ ] Recognize all payload types
- [ ] Expose Discord's REST API through simple method calls
	- [ ] Channel endpoins
	- [ ] Guild endpoints
	- [ ] User endpoints
	- [ ] Invite endpoints
	- [ ] Emoji endpoints
	- [ ] Voice endpoints
	- [ ] Audit log endpoints
- [ ] Caching
	- [ ] ???
- [ ] ???

### 3. How do I build it?

```
git submodules update --init --recursive
make vendor # only needed first build - just run 'make' to compile changes
make
# You can then run it like this
./bin/babble
```
