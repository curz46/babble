## Babble

### 1. What is it?

Babble is a Discord library written in C. It was created due to the void of
approachable, feature-rich libraries which are abundant in languages like Java
and JavaScript.

### 2. Where is it? (roadmap)

- [ ] *Gateway*
	- [x] Authorization
	- [ ] *Recognize all `DISPATCH` payloads*
- [ ] *Expose Discord's REST API through simple method calls*
    - [ ] *http*
        - [x] Implement http `GET`, `PUT`, `PATCH`, etc...
        - [x] Verify `http_post` is working
        - [x] Verify `http_patch` is working
        - [ ] Verify `http_get` is working
        - [ ] Verify `http_put` is working
	- [ ] *Channel endpoints*
	- [ ] Guild endpoints
	- [ ] User endpoints
	- [ ] Invite endpoints
	- [ ] Emoji endpoints
	- [ ] Voice endpoints
	- [ ] Audit log endpoints
- [ ] Event bus
- [ ] Library API e.g. `bbl_login`
- [ ] Caching

*Italics* means in-progress.

### 3. How do I build it?

```
git submodules update --init --recursive
make vendor # only needed first build - just run 'make' to compile changes
make
# You can then run it like this
./bin/babble
```
