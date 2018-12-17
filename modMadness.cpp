#include "modMadness.h"

class Module {
 public:
  std::string name;
  std::queue<std::string> inputs;
  int op;

  Module( std::string name, std::string function){
	this->name = name;
        
	if(function.compare("echo") == 0){
		op = 1;
	} else if (function.compare("reverse") == 0){
		op = 2;
	} else if (function.compare("delay") == 0){
		op = 3;
		this->inputs.push( "hello" );
	} else if (function.compare("noop") == 0){
		op = 4;
	} else {
		printf("Error: Bad Module Operation, Set to noop\n");
		op = 4;
	}
  }

};



//returns Nth word of a std::string, returns empty string if there is no Nth word

std::string getNthWord(int n, std::string userInput)
{
  std::string word;
  std::size_t pos1 = 0;
  std::size_t pos2 = 1;

  while(n >= 0)
  {
    if(pos2 >= userInput.length()) return std::string();
    pos2 = userInput.find(" ", pos1);
    word = userInput.substr(pos1, pos2 - pos1);
    pos1 = pos2 + 1;
    n--;

 }

  return word;
}



/////////////////////
//
// GLOBAL VARIABLES:
//
// modules: array of pointers to Module objects, populated in order of module creation
//
// connections: a 2d arry of integers representing connections between modules. connections[0][2] = 5 means module 0 has 3 connections
// with the 3rd being a connection to the module at modules[5]
// connections is initially populated with all entries as -1.
//
// numModules: number of modules added in modules array 
//
///////////////////


Module *  modules [ 100 ];	
int connections [ 100 ] [ 100 ]; 		
int numModules;			



void addModule( std::string name, std::string function){
  modules[numModules] = new Module(name, function);
  numModules++;
}



void connectModule( std::string a, std::string b ){
  int intA = -1;		//index of module with name a in modules array
  int intB = -1; 		//index of module with name b in modules array

  //find intA, intB
  for(int i = 0; i <= numModules; i++){
	if(a.compare(modules[i]->name) == 0){
		intA = i;
	}
	if(b.compare(modules[i]->name) == 0){
		intB = i;
	}

	if(intA != -1 && intB != -1) break;
  }

  //find correct 2nd index.
  int c = 0;
  while(connections[intA][c] != -1 && c < 100){
 	c++;
  }
  connections[intA][c] = intB; 
}



//Processes mod->content according to mod->op, 1=echo, 2=reverse, 3=delay, 4=noop.

std::string doOp( Module * mod ){
  std::string out;	
  if(mod->inputs.empty()) return "";
  switch( mod->op ){
	case 1:
		out =  mod->inputs.front() + " " + mod->inputs.front();
		mod->inputs.pop();
		return out;
	case 2:	
		out = mod->inputs.front();
		for(int i = 0; i < out.length(); i++){
			out[i] = mod->inputs.front()[out.length() - i - 1];
		}
		mod->inputs.pop();
		return out;
	case 3:
		out = mod->inputs.front();
		mod->inputs.pop();
		return out;
	case 4:
		out = mod->inputs.front();
		mod->inputs.pop();
		return out;
		
  }

  return "ERROR: doOp unsuccesful";
}



void processWord( std::string word ){
  std::string postProcess;
  std::queue<int> modQ;
  int currentMod = 0;

  modQ.push( 0 );
  Module * temp = modules[0];
  temp->inputs.push(word);
  
  while(true){
	
	if( modQ.empty() ) { break; }

	temp = modules[modQ.front()];
	currentMod = modQ.front();
	modQ.pop();

	postProcess = doOp( temp );
	int i = 0;

	//if currentMod is at end of chain, process, print, and continue.
	if( connections[currentMod][0] == -1 ){
		if(postProcess.back() == ' ' || postProcess.compare("")==0){
   			printf("%s", postProcess.c_str());
  		} else {
    			printf("%s ", postProcess.c_str());
  		}

		continue;
	}

	//Traverse connections, push them to ModQ for later processing.
	while( connections[currentMod][i] != -1 ){
		if( postProcess.compare("") != 0){
			Module * sendTo = modules[connections[currentMod][i]];
			if( sendTo->inputs.empty() || sendTo->op == 3 ){
				sendTo->inputs.push( postProcess );
			} else {
				if(sendTo->inputs.front().back() == ' '){
					sendTo->inputs.front() += postProcess;
				} else {
					sendTo->inputs.front() += " " + postProcess;
				}
			}
		}

		//Conditional prevents repeated consecutive evaluations of the same mod
		if( modQ.back() != connections[currentMod][i] ) {
			modQ.push( connections[currentMod][i] );
		}

		i++;
	}
  }
}



//returns true if not all modules are empty. Checks by comparison with empty string "", used for clearing modules in processString
bool modulesNotEmpty() {
  for(int i = 0; i < numModules; i++){  
	if(!modules[i]->inputs.empty())
		return true;
  }
  return false;
}



void processString( std::string userInput ){

  int n = 1;
  std::string word = getNthWord( n , userInput );

  while(!word.empty()){
    processWord( word );
    n++;
    word = getNthWord( n , userInput );
  }
  //Clear the modules after all words in string are processed by processing the empty string "" until all mods are empty.
  while(modulesNotEmpty()){ 
     processWord( "" );
   }


  printf("\n");
}



int main(int argc, char * argv[])
{

  std::string userInput;	//stores one whole command; a single line from stndin
  std::string word;		//stores one word within the command at a time
  int n;			//used for getNthWord
  numModules = 0;

  //populate connections with -1
  for(int x = 0; x < 100; x++){
    for(int y = 0; y < 100; y++){
        connections[x][y] = -1;
    }
  }

  while(true) 			//exits when user types "exit\n"
  {
	std::getline(std::cin, userInput);
	if(userInput.compare("exit") == 0) break;

	n = 0;

	word = getNthWord(n, userInput);

	if(word.compare("module") == 0){
		addModule( getNthWord(1, userInput), getNthWord(2, userInput) );		
	} else if (word.compare("connect") == 0){
		connectModule( getNthWord(1, userInput), getNthWord(2, userInput) );		
	} else if (word.compare("process") == 0){
		processString( userInput );		
	} else { 
		printf("improper input try again\n");
	}
  }
}

