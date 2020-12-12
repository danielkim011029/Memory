#include <iostream>  
#include <vector>
using namespace std;

struct node{
	string processName;
	int memory;
	char type;
	int startAdr;
	int endAdr;
};

//Finding hole large enough for memory
int findHole(vector<node*> processes,int memory){
	for(int i=0;i<processes.size();i++){
		if(processes[i]->processName=="FREE"){ //if found hole
			if(processes[i]->memory>=memory){ //checking if its big enough
				return i;					//return the index of the hole
			}	
		}	
	}
	return -1;
}

//Finding start address
int getStartAdr(vector<node*> processes,int index){
	if(index==0) return 0;
	return processes[index-1]->endAdr+1;
}

//Inserting new process into appropriate place then returns the updated process vector
vector<node*> findPlace(vector<node*> processes, node* temp,int index){
	if(index==processes.size()){
		processes.insert(processes.begin()+index,temp);
	}
	else{
		if(temp->memory==processes[index]->memory){
			processes[index]=temp;
		}
		else if(temp->memory<processes[index]->memory){
			processes.insert(processes.begin()+index,temp);
			processes[index+1]->memory=processes[index+1]->memory-temp->memory;
			processes[index+1]->startAdr=temp->endAdr+1;
			
		}
	}
	return processes;
}
//moving a free processes to the end of memory for compaction
vector<node*> moveToLast(vector<node*> processes, int index){
	while(index!=processes.size()-1&&processes[index+1]->processName!="FREE"){
		//swap
		node* temp=processes[index];
		processes[index]=processes[index+1];
		processes[index+1]=temp;
		//update address
		processes[index]->startAdr=processes[index]->startAdr-processes[index+1]->memory;
		processes[index]->endAdr=processes[index]->endAdr-processes[index+1]->memory;
		processes[index+1]->startAdr=processes[index+1]->startAdr+processes[index]->memory;
		processes[index+1]->endAdr=processes[index+1]->endAdr+processes[index]->memory;
		index++;
	}
	return processes;
}

int main(){
	vector<node*> processes;
	string allocator;
	string processName;
	int memory;
	char type;
	int startAdr;
	int endAdr;
	
	//Start of memory block
	node* start=new node();
	start->processName="FREE";
	start->memory=1048576;
	start->startAdr=0;
	start->endAdr=1048576;
	processes.push_back(start);
	
	while(true){
		cout<<"Enter instructions"<<endl;
		cin>>allocator;
		if(allocator=="QUIT") break;
		
		if(allocator=="RQ"){
			cin>>processName>>memory>>type;
			node* temp=new node();
			int index=findHole(processes,memory);	//hole that fits the memory space
			if(index==-1){
				cout<<"Too large"<<endl;
			}
			else{
				temp->processName=processName;
				temp->memory=memory;
				temp->type=type;	
				temp->startAdr=getStartAdr(processes,index);	//assigning start adress
				temp->endAdr=temp->startAdr+memory-1;		//assigning end address
				processes=findPlace(processes,temp,index);		//inserting new process into right place
			}
		}
		
		else if(allocator=="RL"){
			cin>>processName;
			for(int i=0;i<processes.size();i++){
				if(processes[i]->processName==processName){	//if given process matches the process
					processes[i]->processName="FREE";		//set the process as free space
				}
			}
		}
		else if(allocator=="C"){
			vector<int> indexes;
			for(int i=processes.size()-2;i>=0;i--){
				//if its free memory, move to the end of memory
				if(processes[i]->processName=="FREE"){
					processes=moveToLast(processes,i);
				}
			}
			//combining free addresses into one
			for(int i=0;i<processes.size()-1;i++){
				if(processes[i]->processName=="FREE"){
					indexes.push_back(i);
					processes[i+1]->memory=processes[i]->memory+processes[i+1]->memory;	//combining the free spaces into one
					processes[i+1]->startAdr=processes[i]->startAdr;
					
				}
			}
			//deleting useless memory
			for(int i=0;i<indexes.size();i++){
				processes.erase(processes.begin()+indexes[i]);	//erase the extra free space
				//decrementing the indexes after each deletion
				for(int j=i+1;j<indexes.size();j++){
					indexes[j]--;
				}
			}	
		
			
		}
		
		else if(allocator=="STAT"){
			for(int i=0;i<processes.size();i++){
				cout<<"Addresses "<<"["<<processes[i]->startAdr<<" : "<<processes[i]->endAdr<<"] ";
				if(processes[i]->processName!="FREE") cout<<"Process ";
				cout<<processes[i]->processName<<endl;
			}
		
		}
		
		
	}//end of while loop
	
}
