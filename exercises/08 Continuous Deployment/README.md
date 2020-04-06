# Contionuous Integration

## magnificent commit log

![progress](progress.png)

## SSH setup

The following files are required:
+ Server side:
    + `~/.ssh/authorized_keys` with public key of client (CI pipeline host)
+ Client side (pipeline host):
    + `~/.ssh/id_rsa` - private key of client  
      _must match public key of course_ 
    + `~/.ssh/known_hosts` - output of `ssh-keyscan <server IP>`  
      avoids the usual "do u wanna trust dis host" prompt

## CI pipeline

Being the automation-hungry person I am, I of course had to add a new `SERVER_IP` env variable and a sed command to automate editing the `known_hosts` file:
```bash
sed -ri "s/[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}/$SERVER_IP/g" ~/.ssh/known_hosts
```

the editen example pipeline can be found in [.gitlab-ci.yml](.gitlab-ci.yml)

## systemd setup

To demonstrate restarting a running process, we were tasked with creating a systemd unit and running `systemctl --user restart app` through SSH.

## gitlab being weird

Since I decided to use my own laptop as a server, and since the GitLab instance of our school runs on the internal network, a problem of GitLab sometimes not being able to connect to any ports on my internal IP appeared. This was a significant annoyance.

This made me put in a `ping`, an `ssh -T` and a `curl` to test if anything would work in the current pipeline.

