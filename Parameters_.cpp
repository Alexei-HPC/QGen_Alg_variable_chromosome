#include "Parameters_.h"


Parameters_::Parameters_(void)
{
}


Parameters_::~Parameters_(void)
{
}

const char* GetParameter(TiXmlElement* root, string parameter_name)
{
	string result = "";

	TiXmlElement* child = root->FirstChildElement(parameter_name);
	if ( child )
	{
		return child->GetText();
	}
	else
	{
		cout << "Error! Parameter "<<parameter_name<<" not found." <<endl;
		exit(1);
	}
}

int Parameters_::Parameters_Read(string path)	//���������� ����������
{
	cout<<"Parameters reading started"<<endl;
	TiXmlDocument doc;
	if(!doc.LoadFile(path))//������� ������� ����
	{
		cerr << doc.ErrorDesc() << endl;
		exit(1);
		return 1;
	}

	TiXmlElement* root = doc.FirstChildElement();
	if(root == NULL)
	{
		cerr << "Failed to load file: No root element."	<< endl;
		doc.Clear();
		return 1;
	}

	//genetic_alg_iterations = atoi(root->FirstChildElement("genetic_alg_iterations")->GetText());
	genetic_alg_iterations = atoi(GetParameter(root, "genetic_alg_iterations"));
	
	//individ_number = atoi(root->FirstChildElement("individ_number")->GetText());
	individ_number = atoi(GetParameter(root, "individ_number"));
	
	//chromosome_size = atoi(root->FirstChildElement("chromosome_size")->GetText());
	chromosome_size = atoi(GetParameter(root, "chromosome_size"));

	//crossover_probability = atof(root->FirstChildElement("crossover_probability")->GetText());
	crossover_probability = atof(GetParameter(root, "crossover_probability"));
	
	//mutation_probability = atof(root->FirstChildElement("mutation_probability")->GetText());
	mutation_probability = atoi(GetParameter(root, "mutation_probability"));	

	//mutation_number = atoi(root->FirstChildElement("mutation_number")->GetText());
	mutation_number = atoi(GetParameter(root, "mutation_number"));

	//truncation = atoi(root->FirstChildElement("truncation")->GetText());
	truncation = atoi(GetParameter(root, "truncation"));

	//exchange_individs = atoi(root->FirstChildElement("exchange_individs")->GetText());
	exchange_individs = atoi(GetParameter(root, "exchange_individs"));

	//exchange_period = atoi(root->FirstChildElement("exchange_period")->GetText());
	exchange_period = atoi(GetParameter(root, "exchange_period"));

	//mutation_period = atoi(root->FirstChildElement("mutation_period")->GetText());
	mutation_period = atoi(GetParameter(root, "mutation_period"));

	//fain_for_remain_passangers = atoi(root->FirstChildElement("fine_for_remain_passanger")->GetText());
	fine_for_remain_passanger = atoi(GetParameter(root, "fine_for_remain_passanger"));

	//fain_for_lag = atoi(root->FirstChildElement("fine_for_lag")->GetText());
	fine_for_lag = atoi(GetParameter(root, "fine_for_lag"));

	//fine_for_last_remain_passangers = atoi(root->FirstChildElement("fine_for_last_remain_passangers")->GetText());
	fine_for_last_remain_passangers = atoi(GetParameter(root, "fine_for_last_remain_passangers"));

	//������ ������� �� �������� ���������� ����� ����� � � � � ����� ��������������� ����� �� �������
	//wt_no_fine = atoi(root->FirstChildElement("wt_no_fine")->GetText());
	wt_no_fine = atoi(GetParameter(root, "wt_no_fine"));

	//wt_restful = atoi(root->FirstChildElement("wt_restful")->GetText());
	wt_restful = atoi(GetParameter(root, "wt_restful"));

	//wt_max = atoi(root->FirstChildElement("wt_max")->GetText());
	wt_max = atoi(GetParameter(root, "wt_max"));

	//rm_restful = atoi(root->FirstChildElement("rm_restful")->GetText());
	rm_restful = atoi(GetParameter(root, "rm_restful"));

	//rm_upto_max = atoi(root->FirstChildElement("rm_upto_max")->GetText());
	rm_upto_max = atof(GetParameter(root, "rm_upto_max"));

	//f_refuse= atoi(root->FirstChildElement("f_refuse")->GetText());
	f_refuse = atoi(GetParameter(root, "f_refuse"));

	//������ ����� � ������
	//times_filename = root->FirstChildElement("times_filename")->GetText();
	times_filename = GetParameter(root, "times_filename");

	//density_filename = root->FirstChildElement("density_filename")->GetText();
	density_filename = GetParameter(root, "density_filename");

	//routes_filename = root->FirstChildElement("routes_filename")->GetText();
	routes_filename = GetParameter(root, "routes_filename");

	//costs_filename = root->FirstChildElement("costs_filename")->GetText();
	costs_filename = GetParameter(root, "costs_filename");

	cout<<"Parameters reading completed"<<endl;
	return 0;
}

int Parameters_::Parameters_Bcasting()//�������� ���������� ����� ������
{
	//�������� ����������
	MPI_Bcast(&genetic_alg_iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&chromosome_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&crossover_probability, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&mutation_probability, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&mutation_period, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&exchange_period, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&exchange_individs, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&truncation, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&individ_number, 1, MPI_INT, 0, MPI_COMM_WORLD);
	return 0;
}

int Parameters_::Print()//����� ����������
{
	cout<<"Parameters: "<<endl;

	cout<<"genetic_alg_iterations: "<< genetic_alg_iterations<<endl;	// ����� �������� ������������� ���������
	cout<<"individ_number: "<< individ_number<<endl;	//����� ������
	cout<<"chromosome_size: "<< chromosome_size<<endl;	//����� ���������, ���������� �����
	cout<<"crossover_probability: "<<crossover_probability<<endl;	//����������� �����������
	cout<<"mutation_probability: "<<mutation_probability<<endl;	//����������� �������
	cout<<"mutation_number: "<<mutation_number<<endl;	//����� ���������� �����
	cout<<"exchange_period: "<<exchange_period<<endl;	//������ ����� �������� ���������� ����� �������
	cout<<"exchange_individs: "<<exchange_individs<<endl;	//����� ������ ��� ������
	cout<<"mutation_period: "<<mutation_period<<endl;	//������, ����� ������� ���������� �������
	cout<<"truncation: "<<truncation<<endl;	//�������� ��������
	cout<<"density_filename: "<<density_filename<<endl;		//���� � ����� � ������� � �������� ���������� �� ���������
	cout<<"times_filename: "<<times_filename<<endl;		//���� � ����� � ������� � ������, ������� ���������� ��� ����������� ���� ����� �����������
	cout<<"costs_filename: "<<costs_filename<<endl;		//���� � ����� � ������� � ����� ������� �������� ����� ����������� � ��������� �����
	cout<<"routes_filename: "<<routes_filename<<endl;		//���� � ����� � ������� � ���������
	cout<<"parameters_filename: "<<parameters_filename<<endl;	 //���� � ����� � �����������
	cout<<"uni_path: "<<uni_path<<endl;	 //���� � ����������� �����

	cout<<"fain_for_remain_passangers: "<<fine_for_remain_passanger<<endl;	
	cout<<"fain_for_lag: "<<fine_for_lag<<endl;	
	cout<<"fine_for_last_remain_passangers: "<<fine_for_last_remain_passangers<<endl;	

	cout<<"wt_no_fine: "<<wt_no_fine<<endl;	//�������� "�����������" �������� (���� �������� ��� ������ ��� ����� ����� �������, �� �� ��������);
	cout<<"wt_restful: "<<wt_restful<<endl;		//�������� "������������ ����������" �������� (������� �� ������� ������� ��������� �� ���������);
	cout<<"rm_restful: "<<rm_restful<<endl;		//������ ������ �� ������ �������� ����� "�����������",  �� �� ����� "������������ ����������";
	cout<<"wt_max: "<<wt_max<<endl;	//��������  "�������������" �������� (����� ���� �������� �������������, �� ��������� ��������� ����� � �� ������������ �� �������), ������� �� ������� ������� ��������� �� ���������;
	cout<<"rm_upto_max: "<<rm_upto_max<<endl;	//������ ������ �� ������ �������� ����� "������������ ����������" ��������� ��������,  �� �� ����� "�������������";
	cout<<"f_refuse: "<<f_refuse<<endl;		//������ ������ �� ����� ��������� �� �������.

	return 0;
}
