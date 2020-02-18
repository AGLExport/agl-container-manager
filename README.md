# agl container manager (draft version)

The repository source code is an experimental implementation for research purposes.  
We will formally implement it after deciding the requirements based on this experiment.  

---

# How to create config  

The configuration file conforms to the json format. See other documents for json syntax.  

Example of draft agl-container-manager config.

	{
		"name": "guest-wayland",
		"setting": {
			"rootfs": {
				"path": "/lxc/guests/guest-wayland"
			},
			"lifecycle": {
				"halt": "SIGRTMIN+3",
				"reboot": "SIGTERM"
			},
			"cap": {
				"drop": "sys_module mac_admin mac_override sys_time",
				"keep": ""
			},
			"environment": [
				"XDG_RUNTIME_DIR=/lxc/share"
			]
		},
		"resource": [
			{
				"type":		"cgroup",
				"object":	"cpuset.cpus",
				"value":	"0-1"
			}
		],
		"mount": [
			{
				"type": 	"auto",
				"param": 	"cgroup:mixed proc:mixed sys:mixed"
			},
			{
				"type": 	"filesystem",
				"from": 	"shm",
				"to":		"/dev/shm",
				"fstype":	"tmpfs",
				"option": 	"defaults 0 0"
			},
			{
				"type": 	"directry",
				"from": 	"/sys/kernel/security",
				"to":		"sys/kernel/security",
				"fstype":	"none",
				"option": 	"ro,bind,optional 0 0"
			},
			{
				"type": 	"device",
				"from": 	"/dev/dri",
				"to":		"dev/dri",
				"devnode":	"/dev/dri/card0",
				"fstype":	"none",
				"option": 	"bind,optional,create=dir"
			},
			{
				"type": 	"directry",
				"from": 	"/lxc/share",
				"to":		"lxc/share",
				"fstype":	"none",
				"option": 	"bind,optional,create=dir"
			}
		],
		"network": [
			{
				"type": 	"veth",
				"link": 	"lxcbr0",
				"flags":	"up",
				"hwaddr":	"00:16:3e:xx:xx:xx",
				"veth": {
					"mode": "bridge"
				},
				"ipv4": {
					"address":	"10.0.3.20/24",
					"gateway":	"10.0.3.1"
				},
				"ipv6": {
				}
			},
			{
				"type": 	"can",
				"flags":	"up",
				"can": {
					"hostdev" : "guest0",
					"rules": {
						"receive": [
							"-e"
						],
						"send": [
							"-e"
						]
					}
				}
			}
		],
		"autoboot": true
	}

## "name" section  

	"name": "guest-wayland",

This section write the name of the container.  
This name is used when logging into the container with "lxc-attach -n name" command.  


## "setting" section  

	"setting": {
		"rootfs": {
			"path": "/lxc/guests/guest-wayland"
		},
		"lifecycle": {
			"halt": "SIGRTMIN+3",
			"reboot": "SIGTERM"
		},
		"cap": {
			"drop": "sys_module mac_admin mac_override sys_time",
			"keep": ""
		},
		"environment": [
			"XDG_RUNTIME_DIR=/lxc/share"
		]
	},

A setting section have a 4 type of subsection such as "rootfs", "lifecycle", "cap" and "environment".  

### "rootfs" subsection  

The "rootfs" subsection have a "path" key.  The "path" key write rootfs path of the guest container.  

### "lifecycle" subsection  

The "lifecycle" subsection have a "halt" and "reboot" key.  
When guest container use systemd, these key should keep this settings.  

*Appendix. When guest container don't use systemd or don't use glibc, these key should modify.*  

### "cap" subsection  

The "cap" subsection have a "keep" and "drop" key.  
See "CAPABILITIES" section of [lxc.containe.conf][url-lxc.container.conf.en] for configuration details.  

*Appendix. Japanese version of [lxc.containe.conf][url-lxc.container.conf.jp]*  

### "environment" sub section  

The "environment" subsection writes the array of environment variable.  
These environment variable inherit to guest container.  

## "setting" section  

	"resource": [
		{
			"type":		"cgroup",
			"object":	"cpuset.cpus",
			"value":	"0-1"
		}
	],


## "setting" section  

	"mount": [
		{
			"type": 	"auto",
			"param": 	"cgroup:mixed proc:mixed sys:mixed"
		},
		{
			"type": 	"filesystem",
			"from": 	"shm",
			"to":		"/dev/shm",
			"fstype":	"tmpfs",
			"option": 	"defaults 0 0"
		},
		{
			"type": 	"directry",
			"from": 	"/sys/kernel/security",
			"to":		"sys/kernel/security",
			"fstype":	"none",
			"option": 	"ro,bind,optional 0 0"
		},
		{
			"type": 	"device",
			"from": 	"/dev/dri",
			"to":		"dev/dri",
			"devnode":	"/dev/dri/card0",
			"fstype":	"none",
			"option": 	"bind,optional,create=dir"
		},
		{
			"type": 	"directry",
			"from": 	"/lxc/share",
			"to":		"lxc/share",
			"fstype":	"none",
			"option": 	"bind,optional,create=dir"
		}
	],


## "setting" section  

	"network": [
		{
			"type": 	"veth",
			"link": 	"lxcbr0",
			"flags":	"up",
			"hwaddr":	"00:16:3e:xx:xx:xx",
			"veth": {
				"mode": "bridge"
			},
			"ipv4": {
				"address":	"10.0.3.20/24",
				"gateway":	"10.0.3.1"
			},
			"ipv6": {
			}
		},
		{
			"type": 	"can",
			"flags":	"up",
			"can": {
				"hostdev" : "guest0",
				"rules": {
					"receive": [
						"-e"
					],
					"send": [
						"-e"
					]
				}
			}
		}
	],

[url-lxc.container.conf.en]:https://linuxcontainers.org/lxc/manpages/man5/lxc.container.conf.5.html
[url-lxc.container.conf.jp]:https://linuxcontainers.org/ja/lxc/manpages/man5/lxc.container.conf.5.html

