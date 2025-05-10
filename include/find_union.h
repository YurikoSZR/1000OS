#ifndef _UNION_FIND_
#define _UNION_FIND_
template<class T>
struct union_find{
	std::vector<T> element;
	std::vector<int> index;
	int count=0;
	void insert(T E){
		element.push_back(element);
		index.push_back(coun++);
	};

	int find(int entry)
	{
		if(entry==index[entry])return entry;
		index[entry]=find(index[entry]);
		return index[entry];
	
	}
	void merge(int x,int y){
		index[find(y)]=index[find(x)];
	}
	
};





#endif
