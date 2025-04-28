#include <iostream>
#include <vector>
#include <set>

int main(){
	std::set<int> TEST;
	TEST.insert(3);
	TEST.insert(3);
	TEST.insert(4);
	TEST.insert(5);
	for(auto item=TEST.begin();item!=TEST.end();++item){
		std::cout<<*item<<std::endl;
	}

}
