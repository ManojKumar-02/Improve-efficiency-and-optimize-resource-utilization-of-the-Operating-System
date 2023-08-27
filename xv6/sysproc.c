#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// New system call
int 
sys_mydate(struct rtcdate * r)
{
  char month[12][15] = {"January","Febraury","March","April","May","June","July","August","September","October","November","December"};

  if(argptr(0,(void*)&r,sizeof(&r)) < 0) 
  return -1;
  cmostime(r);
    cprintf("Year: %d\n",r->year);
    cprintf("Month: %d or %s\n",r->month,month[r->month-1]);
    cprintf("Date: %d\n",r->day);
    cprintf("Hour: %d\n",r->hour);
    cprintf("Minute: %d\n",r->minute);
    cprintf("Second: %d\n",r->second);
  return 0;
}

int 
sys_mypgtPrint(void)
{
 	struct proc *current = myproc();
	pde_t* pagedir = current->pgdir;
	int entry = 0;
	
	//Interating on each PDE in page directory
        for(int i=0; i<NPDENTRIES; i++){
        	//Checing for valid page directory
                if(pagedir[i] != 0){
                        pte_t* pgtabEn = (pte_t *)P2V(PTE_ADDR(pagedir[i]));
                        
                        //If PDE is present then iterate over each PTE in page table 
                        for(int j=0; j<NPTENTRIES; j++){
                                if((pgtabEn[j] & PTE_P) && (pgtabEn[j] & PTE_U)&& pgtabEn[j]){ 
   					entry++;					
                                        cprintf("Entry No. : %d, ",entry);
                                        cprintf("Virtual address : %x, ",PGADDR(i, j, 0));
                                        cprintf("the Physical address is : %x\n",PTE_ADDR(pgtabEn[j]));
                                }
                        }
                }
        }
        return 0;
}	





















