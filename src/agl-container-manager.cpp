#include <stdio.h>

#include "ContainerConfig.hpp"
#include "lxc-runtime.hpp"

#include <iostream>

#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signalfd.h>

//---------------------------------------------------------------------------------------------------------------
int signal_setup(void)
{
	int fd = -1;
	sigset_t mask;
	
	// signal mask
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGPIPE);
	::sigaddset(&mask, SIGTERM);
	
	::pthread_sigmask(SIG_BLOCK, &mask, NULL);
	
	//setup signal fd
	::sigemptyset(&mask);
	::sigaddset(&mask, SIGTERM);
	
	// create signal fd
	fd = ::signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
	
	return fd;
}
#if 0
//for test
#include <sys/select.h>
#include <netlink/cli/utils.h>
#include <netlink/cli/link.h>
//for test
//---------------------------------------------------------------------------------------------------------------
static struct nl_sock *g_pnlsock;
static void obj_input(struct nl_object *obj, void *arg)
{
	struct nl_dump_params dp;
	
	::memset(&dp,0,sizeof(dp));
	dp.dp_type = NL_DUMP_STATS;
	dp.dp_fd = stdout;
	dp.dp_dump_msgtype = 1;

	nl_object_dump(obj, &dp);
}
static int event_input(struct nl_msg *msg, void *arg)
{
	if (nl_msg_parse(msg, &obj_input, NULL) < 0)
		fprintf(stderr, "<<EVENT>> Unknown message type\n");

	/* Exit nl_recvmsgs_def() and return to the main select() */
	return NL_STOP;
}
//---------------------------------------------------------------------------------------------------------------
int netlink_setup(void)
{
	int fd = -1;
	struct rtnl_link *link;
	
	g_pnlsock = ::nl_cli_alloc_socket();
	if (g_pnlsock == NULL) return -1;
	
	::nl_socket_disable_seq_check(g_pnlsock);
	::nl_socket_modify_cb(g_pnlsock, NL_CB_VALID, NL_CB_CUSTOM, event_input, NULL);

	::nl_cli_connect(g_pnlsock, NETLINK_ROUTE);
	
	link = ::nl_cli_link_alloc();

	::nl_socket_add_membership(g_pnlsock, RTNLGRP_LINK);
	
	fd = nl_socket_get_fd(g_pnlsock);
	
	return fd;
}
#else
#include <netlink/netlink.h>
#include <netlink/cache.h>
#include <netlink/cli/utils.h>
//---------------------------------------------------------------------------------------------------------------
static struct nl_cache_mngr *g_mngr;
static struct nl_cache *g_cache;
static struct nl_dump_params dp;

static void cache_test(struct nl_cache *cache)
{
    struct rtnl_link *filter;
    struct rtnl_link *link_obj;
    int flags, up;
    char *ifname;
    struct nl_object *obj;

    filter = rtnl_link_alloc();
    if (!filter) {
        fprintf(stderr, "error allocating filter\n");
        return;
    }

	obj = nl_cache_get_first(cache);
	while(obj != NULL)
	{
		if (nl_object_match_filter (obj, OBJ_CAST (filter)) != 0)
		{
		    link_obj = (struct rtnl_link *) obj;
		    flags = rtnl_link_get_flags (link_obj);
		    ifname = rtnl_link_get_name(link_obj);
		
		    printf("IFNAME = %s\n", ifname);
		}
		obj = nl_cache_get_next(obj);
	}

    rtnl_link_put(filter);
}

static void iface_mon_handler2(struct nl_object *obj)
{
    struct rtnl_link *filter;
    struct rtnl_link *link_obj;
    int flags, up;
    char *ifname;

    filter = rtnl_link_alloc();
    if (!filter) {
        fprintf(stderr, "error allocating filter\n");
        return;
    }

    if (nl_object_match_filter (obj, OBJ_CAST (filter)) == 0)
    {
        rtnl_link_put(filter);
        return;
    }

    link_obj = (struct rtnl_link *) obj;
    flags = rtnl_link_get_flags (link_obj);
    ifname = rtnl_link_get_name(link_obj);
    //up = (flags & IFF_UP) ? 1 : 0;

    printf("IFNAME = %s\n", ifname);

    rtnl_link_put(filter);

    return;
}

static void change_cb(struct nl_cache *cache, struct nl_object *obj,
		      int action, void *data)
{
	struct nl_dump_params dp;
	
	::memset(&dp,0,sizeof(dp));
	dp.dp_type = NL_DUMP_LINE;
	dp.dp_fd = stdout;

	if (action == NL_ACT_NEW)
		printf("NEW ");
	else if (action == NL_ACT_DEL)
		printf("DEL ");
	else if (action == NL_ACT_CHANGE)
		printf("CHANGE ");

//	::nl_object_dump(obj, &dp);
	iface_mon_handler2(obj);
	//::nl_cache_mngr_info(g_mngr, &dp);
}
int items;
//---------------------------------------------------------------------------------------------------------------
int netlink_setup(void)
{
	int fd = -1;
	int err = -1;
	struct nl_sock *sock;
	
	sock = ::nl_cli_alloc_socket();

	err = ::nl_cache_mngr_alloc(sock, NETLINK_ROUTE, NL_AUTO_PROVIDE, &g_mngr);
	if (err < 0) return -1;
	
	err = ::nl_cache_mngr_add(g_mngr, "route/link", &change_cb, NULL, &g_cache);
	if (err < 0) return -1;
	
	fd = nl_cache_mngr_get_fd(g_mngr);
	
	cache_test(g_cache);
	
	return fd;
}


#endif
//---------------------------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) 
{
	int sigfd = -1, netlinkfd = -1, ret = -1;
	struct pollfd pfds[2];
	struct signalfd_siginfo fdsi;
	std::vector< std::string > vstr;
	
	sigfd = signal_setup();
	netlinkfd = netlink_setup();
	
	
	// setup poll
	::memset(pfds,0,sizeof(pfds));
	pfds[0].fd = sigfd;
	pfds[0].events = POLLIN;
	pfds[0].revents = 0;
	pfds[1].fd = netlinkfd;
	pfds[1].events = POLLIN;
	pfds[1].revents = 0;
	
	CLXCRuntime *pLXC = new CLXCRuntime();
	
	pLXC->GetGuestList(vstr);
	
	for(int i=0;i<vstr.size();i++)
	{
		std::cout << "exec: " << vstr[i] << std::endl;
		pLXC->ExecGuestContainer(vstr[i]);
	}
	
	
	do
	{
		ret = poll(pfds, sizeof(pfds) / sizeof(struct pollfd), -1);
		
		if (ret == 1)
		{
			ret = read(pfds[0].fd, &fdsi, sizeof(struct signalfd_siginfo));
			if (ret == sizeof(struct signalfd_siginfo))
			{
				if ( fdsi.ssi_signo == SIGTERM)
				{
					break;
				}
			}
			
			//::nl_recvmsgs_default(g_pnlsock);
			::nl_cache_mngr_data_ready(g_mngr);
		}
		else 
		{
			//EINTR,EFAULT
			//EINVAL,ENOMEM
			if ((errno == EINVAL) || (errno == ENOMEM))
			{
				struct timespec wait;
				wait.tv_sec  = 0;
				wait.tv_nsec = 100 * 1000 * 1000;
				
				::nanosleep(&wait, NULL);
			}
		}
	
	}
	while(1);
	
	for(int i=0;i<vstr.size();i++)
	{
		std::cout << "shoutdown: " << vstr[i] << std::endl;
		pLXC->ShoutdownGuestContainer(vstr[i]);
	}
	
	
	delete pLXC;
	
	return 0;
}
