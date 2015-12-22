#include"regex.cpp"
#include<iostream>
extern vector< vector<int> > findall(const char *regex,const char *content);
int main() {
	char regex[] = "([a-z])";
	char content[] = "asds";
	vector<vector<int> > result;
	result = findall(regex, content);
	for (int i = 0; i < result.size(); i++) {
		for (int j = result[i][0]; j < result[i][1]; j++) {
			printf("%c", *(content+j));
		}
		printf(":");
		for (int j = 0; j < result[0].size(); j += 2)
		{
			printf("(%d,%d)", result[i][j], result[i][j+1]);
		}
		printf("\n");
	}
	printf("\n");
}