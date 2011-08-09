
#include "poll.h"

PollSet::PollSet(const int max): maxfds(max){
  ufds = new struct pollfd[max];
  nactive = 0;
}

PollSet::~PollSet(){
  delete[] ufds;
}

int PollSet::where(socket_t x){
  for (int i=0;i<maxfds;i++)
    if (ufds[i].fd == x) return i;
  assert(0);
}

void PollSet::add(const socket_t fd){
  omni_mutex_lock sync(guard);
  assert(nactive < maxfds);

  // make this operation idempotent by allowing an fd to be
  // added more than once
  for (int i=0;i<nactive;i++)
    if (ufds[i].fd == fd) return;

  ufds[nactive].fd = fd;
  ufds[nactive].events = POLLIN | POLLERR | POLLHUP;
  ufds[nactive].revents = 0;
    
  nactive ++;
}

void PollSet::remove(const socket_t fd){
  omni_mutex_lock sync(guard);

  assert(nactive > 0);
  for(int i=0;i<maxfds;i++)
    if (ufds[i].fd == fd){
      for (int j=i+1;j<nactive;j++)
	ufds[j-1] = ufds[j];
      nactive--;
      return;
    }
  // it wasnt active!
  abort();
}

ostream &operator << (ostream& s, const PollSet& set){
  omni_mutex_lock sync(set.guard);

  s << "PollSet nactive = " << set.nactive << " Requested events on [";
  for (int i=0;i<set.nactive;i++)
    s << set.ufds[i].fd << " ";
  s << "] Received on [";
  for (int i=0;i<set.nactive;i++)
    if (set.ufds[i].revents) s << set.ufds[i].fd << " ";
  s << "]";
  return s;  
}

PollSet_Iterator::PollSet_Iterator(PollSet &ps): set(ps){
  //set = ps;
  index = ps.nactive - 1;
}

socket_t PollSet_Iterator::operator() (){
  if (index == -1)
    return SOCKET_UNDEFINED;

  return set.ufds[index--].fd;
}

PollSet_Active_Iterator::PollSet_Active_Iterator(PollSet &ps):
  PollSet_Iterator(ps){
}

socket_t PollSet_Active_Iterator::operator() (){
  while ((index > -1) && (! (set.ufds[index].revents))){
    index--;
  }
  if ((index > -1) && (set.ufds[index].revents)){
    return set.ufds[index--].fd;
  }
  return SOCKET_UNDEFINED;
}
ostream &operator <<(ostream& s, const Poller& ps){
  return s << ps.ps;
}

Poller::Poller(): ps(MAXFDS), state(awake), nthreads_waiting(0),
                  wakeUP_cond(&state_guard) {
  pipe_fd = new socket_t[2];
  if (pipe(pipe_fd) != 0){
    perror("Unable to open internal pipe");
    exit(1);
  }
  ps.add(pipe_fd[0]);
}

void Poller::wakeUp(){
  char mesg = '!';
  write(pipe_fd[1], &mesg, sizeof(char));
}

void Poller::add(socket_t x){
  omni_mutex_lock s(state_guard);
  if (state == awake){
    // safe to modify PollSet
    ps.add(x);
    return;
  }
  // wait until nthreads_waiting >=0
  while (nthreads_waiting < 0) wakeUP_cond.wait();
    
  wakeUp();
  nthreads_waiting ++;
  // control thread will flip the sign when its ready
  while (nthreads_waiting > 0) wakeUP_cond.wait();
  
  ps.add(x);
  nthreads_waiting ++;
  wakeUP_cond.broadcast();
  
  // this ones done
}

void Poller::remove(socket_t x){
  omni_mutex_lock s(state_guard);
  if (state == awake){
    // safe to modify PollSet
    ps.remove(x);
    return;
  }
  // wait until nthreads_waiting >=0
  while (nthreads_waiting < 0) wakeUP_cond.wait();
  
  wakeUp();
  nthreads_waiting ++;
  // control thread will flip the sign when its ready
  while (nthreads_waiting > 0) wakeUP_cond.wait();
  
  ps.remove(x);
  nthreads_waiting ++;
  wakeUP_cond.broadcast();
  
  // this ones done
}

PollSet_Active_Iterator Poller::wait(){
  while(1){
    // about to block...
    {
      omni_mutex_lock s(state_guard);
      state = asleep;
    }

    while (1){
      // clean the return values?
      for (int i=0;i<ps.nactive;i++) ps.ufds[i].revents = 0;

      int result = poll(ps.ufds, ps.nactive, -1);
      if (result == -1){
	if (errno == EINTR) continue;
	perror("poll() returned an error");
	exit(1);
      }
      if (result == 0){
	perror("poll() timed out?!");
	exit(1);
      }
      if (result > 0) break;
    }

    {
      omni_mutex_lock s(state_guard);
      state = awake;
      int index = ps.where(pipe_fd[0]);
      if (ps.ufds[index].revents){
	char temp;
	if (read(pipe_fd[0], &temp, sizeof(char)) != 1){
	  perror("Error reading from control pipe");
	  exit(1);
	}
	// someone signalled us.. they're now blocked on
	// the condition variable
	nthreads_waiting =- nthreads_waiting;
	wakeUP_cond.broadcast();
	// it's safe for them to do things now... wait 
	// for them all to finish
	while (nthreads_waiting < 0) wakeUP_cond.wait();
	// they've all finished updating my state
	ps.ufds[index].revents = 0;
	// round the loop again
	continue;
      }
      ps.ufds[index].revents = 0;
    }
    return PollSet_Active_Iterator(ps);
  }
}