#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include "time_counter.h"
#include "DA2dims.h"
#include "EPNAR_Process.h"
#include <vector>
using namespace std;

void classify()
{
	/* thuat toan thuc hien phan lop dua tren luat am duong mo rong phan biet*/
	cout << "CHUONG TRINH PHAN LOP DUA TREN TAP AM DUONG MO RONG PHAN BIET \n";

	////////////////////////////////////////////////////////////
	string filename;
	cout << "nhap vao ten file du lieu huan luyen:";
	cin >> filename;
	//int temp;
	// read data file
	DA2dims data;
	data.bi_dataimport(filename);
	//cout << data.To_String();
	//
	time_counter t;
	t.settime();
	//scanf("%d",&temp);
	int time = t.running_period();
	///////////////////////////////////////////////////////////////
	EPNAR_Process* pro = new EPNAR_Process(&data, data.getcolnum(), data.getrownum());
	(*pro).printData();
	double minsup, minconf;
	unsigned int Y;
	cout << endl;
	cout << " Nhap vao minsup: "; cin >> minsup;
	cout << " Nhap vao minconf: "; cin >> minconf;
	cout << " Thu tu bien target: "; cin >> Y;
	string testfilename;
	cout << "nhap vao ten file test:";
	cin >> testfilename;
	///////////////////////////////////////////////////////////
	// cau hinh luat
	bool Ruleform = true;
	bool RedunCon = false;

	(*pro).generateK1rule(minsup, minconf, Y, Ruleform, RedunCon);
	cout << "luat duoc tim thay " << endl;
	cout << "Luat noi chung: " << endl;
	(*pro).printpolularRules();
	if (!RedunCon)
		(*pro).printvalidRules();
	cout << endl << " Luat khong du thua " << endl;
	(*pro).printvalidNotRedundantRules();
	/// sort and print again
	cout << endl << "Luat valid duoc sap xep lai theo order." << endl;
	(*pro).rulessort();
	if (!RedunCon)
		(*pro).printvalidRules();
	(*pro).printvalidNotRedundantRules();
	//////////////////////////////////////////////////////////////
	
	DA2dims test_data;
	test_data.bi_dataimport(testfilename);
	//cout << test_data.To_String();
	cout << "ket qua clasification: " << endl;
	int numberofclasifiedInstance = 0;
	int numberofNotclasifiedInstance = 0;
	int truePositive = 0; int falsePositive = 0; int trueNegative = 0; int falseNegative = 0;
	bool classification_rule_type = true; // using valid and not redundant rules
	for (unsigned int i = 0; i < test_data.getrownum(); i++)
	{
		vector<unsigned int> InputTrans = test_data.getrow(i);
		epnAR rule; int Y_value;
		bool selectedflag = (*pro).selectedrule(InputTrans, &rule, &Y_value, classification_rule_type);
		if (selectedflag)
		{
			cout << "input " << i << " co luat phu hop: " << " y= " << Y_value << endl;
			numberofclasifiedInstance++;
			if (Y_value == test_data.GetSpecEle(Y, i))
			{
				if (Y_value == 1) truePositive++;
				else trueNegative++;
			}
			else
			{
				if (Y_value == 1) falsePositive++;
				else falseNegative++;
			}
		}
		else
		{
			numberofNotclasifiedInstance++;
			cout << " input " << i << " khong co luat phu hop" << endl;
		}
	}
	//
	printf("\ngio chay chuong trinh la: %d \n", t.running_period() - time);
	cout << "So phan lop: " << numberofclasifiedInstance << endl;
	cout << "so khong phan lop: " << numberofNotclasifiedInstance << endl;
	cout << "True Positive: " << truePositive << endl;
	cout << "False Positive: " << falsePositive << endl;
	cout << "True Negative: " << trueNegative << endl;
	cout << "False Negative: " << falseNegative << endl;
	system("pause");
}

void NDPARsMiner()
{
	/* thuat toan thuc hien tim luat am duong mo rong phan biet sap xep va luu ra file*/
	cout << "CHUONG TRINH TIM TAP LUAT AM DUONG MO RONG PHAN BIET \n";

	////////////////////////////////////////////////////////////
	string filename;
	cout << "nhap vao ten file du lieu:";
	cin >> filename;
	//int temp;
	// read data file
	DA2dims data;
	data.bi_dataimport(filename);
	//cout << data.To_String();
	//
	time_counter t;
	t.settime();
	//scanf("%d",&temp);
	int time = t.running_period();
	///////////////////////////////////////////////////////////////
	EPNAR_Process* pro = new EPNAR_Process(&data, data.getcolnum(), data.getrownum());
	(*pro).printData();
	double minsup, minconf;
	unsigned int Y;
	cout << endl;
	cout << " Nhap vao minsup: "; cin >> minsup;
	cout << " Nhap vao minconf: "; cin >> minconf;
	cout << " Thu tu bien target: "; cin >> Y;
	
	///////////////////////////////////////////////////////////
		// cau hinh luat
	bool Ruleform = true;
	bool RedunCon = false;

	(*pro).generateK1rule(minsup, minconf, Y, Ruleform, RedunCon);
	cout << "luat duoc tim thay " << endl;
	cout << "Luat noi chung: " << endl;
	(*pro).printpolularRules();
	if (!RedunCon)
		(*pro).printvalidRules();
	cout << endl << " Luat khong du thua " << endl;
	(*pro).printvalidNotRedundantRules();
	/// sort and print again
	cout << endl << "Luat valid duoc sap xep lai theo order." << endl;
	(*pro).rulessort_for_var_selection();
	if (!RedunCon)
		(*pro).printvalidRules();
	(*pro).printvalidNotRedundantRules();
	//////////////////////////////////////////////////////////////
	string savefilename;
	cout << "nhap vao ten file lu tru:";
	cin >> savefilename;
	string DPNARs_file = savefilename + "_valid.txt";
	(*pro).fprintvalidRules(DPNARs_file);
	string Notredundant_file = savefilename + "_valid_Not_redun.txt";
	(*pro).fprintvalidNotRedundantRules(Notredundant_file);

}

int _tmain(int argc, _TCHAR * argv[])
{
	bool classification_flag = true;
	if (classification_flag)
	{
		// Tim luat va thuc hien phan lop
		classify();
	}
	else
	{
		////////////////////////////////////////////////////////////
		// tim luat va luu ra  file
		NDPARsMiner();
	}
	system("pause");
	return 0;
}
