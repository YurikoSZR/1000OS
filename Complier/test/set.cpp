#include <iostream>
#include <vector>
#include <set>

// int main(){
// 	std::set<int> TEST;
// 	TEST.insert(3);
// 	TEST.insert(3);
// 	TEST.insert(4);
// 	TEST.insert(5);
// 	for(auto item=TEST.begin();item!=TEST.end();++item){
// 		std::cout<<*item<<std::endl;
// 	}

// }

int main(){
	std::set<int> test_1;
	test_1.insert(3);
	std::set<int> test_2;
	test_2.insert(4);
	std::set<std::set<int>> UNIVERSAL;
	std::set<int> test_3;
	test_3.insert(3);
	UNIVERSAL.insert(test_1);
	UNIVERSAL.insert(test_2);
	UNIVERSAL.insert(test_3);
	for(auto item=UNIVERSAL.begin();item!=UNIVERSAL.end();++item){
		for(auto item_2=item->begin();item_2!=item->end();++item_2){
			std::cout<<*item_2<<std::endl;
		}
	}
	
}
