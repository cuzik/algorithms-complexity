#include "rsa.h"

#define SIZE_BLOCK 2

RSA::RSA(){
	//
}

void RSA::generate_keys(BigInteger p, BigInteger q){
	this->p = p;
	this->q = q;
	this->n = p*q;
	this->alpha = (p-1)*(q-1);
	this->e = choose_e();
	this->d = choose_d(this->e,this->alpha);
}

int RSA::choose_e(){
	return choose_a_small_prime_number();
}

//Do this functions to use later
int RSA::choose_a_small_prime_number(){
	return 227;
	//crivo of Eratóstenes, i dont now...
}

BigInteger RSA::probable_prime_number(int bits){
	return 0;
}

bool RSA::is_prime_number(BigInteger number){
	return true;
}

BigInteger RSA::choose_d(BigInteger e, BigInteger n){
	return modular_reverse(e,n);
}

BigInteger RSA::modular_reverse(BigInteger a, BigInteger b){
	Trio t = extended_euclid(a,b);
	return mod_BI(t.x,b);
}

Trio RSA::extended_euclid(BigInteger a, BigInteger b){
	Trio t;
	if(a<b){
		BigInteger aux = a;
		a = b;
		b = aux;
	}
	
	BigInteger m1 = 1;
	BigInteger m2 = 0;
	BigInteger n1 = 0;
	BigInteger n2 = 1;
	BigInteger res = 0;
	BigInteger quo = 0;
	
	while(b!=0){
		res = mod_BI(a,b);
		quo = a / b;
		a = b;
		b = res;
		if(b==0){
			break;
		}
		t.y = m1-m2*quo;
		t.x = n1-n2*quo;
		m1 = m2;
		m2 = t.y;
		n1 = n2;
		n2 = t.x;
	}
	t.mdc = a;
	return t;
}

BigInteger RSA::mdc(BigInteger a, BigInteger b){
	if(b==0)
		return a;
	else
		return mdc(b,mod_BI(a,b));
}

BigInteger RSA::get_p(){
	return this->p;
}
BigInteger RSA::get_q(){
	return this->q;
}
BigInteger RSA::get_n(){
	return this->n;
}
int RSA::get_e(){
	return this->e;
}
BigInteger RSA::get_d(){
	return this->d;
}

void RSA::encrypt(std::string input, std::string output, BigInteger n, BigInteger e){
	std::string message = read_file(input);
	std::vector<std::string> block_message = split_message(message, SIZE_BLOCK);
	std::vector<std::string> code_bin_number = block_message_2_bin_number(block_message);
	if((code_bin_number.back()).length()!=SIZE_BLOCK*8){
		code_bin_number[code_bin_number.size()-1] = zero_complete(code_bin_number.back(),SIZE_BLOCK*8-(code_bin_number.back()).length());
	}
	std::vector<BigInteger> code_dec_number = block_bin_number_2_dec_number(code_bin_number);
	std::vector<BigInteger> block_encrypt_data = block_dec_number_2_encrypt_data(code_dec_number,n,e);
	write_cipher_in_file(output, block_encrypt_data);
}

std::vector<std::string> RSA::split_message(std::string message, int size_split){
	std::vector<std::string> block_message_return;
	int n=0;
	int size_message = message.length();
	while(n<size_message){
		block_message_return.push_back(std::string(message,n,size_split));
		n+=size_split;
	}
	return block_message_return;
}

std::vector<std::string> RSA::block_message_2_bin_number(std::vector<std::string> block_message){
	std::vector<std::string> code_bin_number_return;
	int i;
	for(i=0;i<(int)block_message.size();i++){
		code_bin_number_return.push_back(string_2_bin_number(block_message[i]));
	}
	return code_bin_number_return;
}
std::string RSA::string_2_bin_number(std::string parcer_message){
	std::string parcer_code_bin_number = "";
	int i;
	for(i=0;i<(int)parcer_message.length();i++){
		parcer_code_bin_number = parcer_code_bin_number + dec_number_2_bin_number((int)parcer_message[i]);
	}
	return parcer_code_bin_number;
}

std::string RSA::dec_number_2_bin_number(BigInteger dec_number){
	std::string bin_number  = "";
	int length = length_bin_number(dec_number);
	BigInteger teste_value = ((int)pow(2,length-1));
	int i;
	for(i=0;i<length;i++){
		if(teste_value <= dec_number){
			bin_number = bin_number + "1";
			dec_number = dec_number - teste_value;
		}else{
			bin_number = bin_number + "0";
		}
		teste_value = teste_value / 2;
	}
	return bin_number;
}

int RSA::length_bin_number(BigInteger dec_number){
	int length_return = 8;
	while(dec_number > (pow_BI(2,length_return)-1)){
		length_return+=8;
	}
	return length_return;
}

BigInteger RSA::pow_BI(BigInteger n, BigInteger a){
	if(a.toInt()==0)
		return 1;
	if(a.toInt()==1)
		return n;
	if(a.toInt()==2)
		return n*n;
	if(mod_BI(a,2).toInt()==1)
		return pow_BI(pow_BI(n,a/2),2)*n;
	return pow_BI(pow_BI(n,a/2),2);
}

BigInteger RSA::pow_BI_Mod(BigInteger n, BigInteger a, BigInteger m){
	if(a.toInt()==0)
		return 1;
	if(a.toInt()==1)
		return mod_BI(n,m);
	if(a.toInt()==2)
		return mod_BI(n,m)*mod_BI(n,m);
	if(mod_BI(a,2).toInt()==1)
		return mod_BI(mod_BI(pow_BI_Mod(mod_BI(pow_BI_Mod(mod_BI(n,m),a/2,m),m),2,m),m)*mod_BI(n,m),m);
	return mod_BI(pow_BI_Mod(mod_BI(pow_BI_Mod(mod_BI(n,m),a/2,m),m),2,m),m);
}

std::string RSA::zero_complete(std::string atual_bin_number, int size_last){
	int i;
	for(i=0;i<size_last;i++){
		atual_bin_number = atual_bin_number + "0";
	}
	return atual_bin_number;
}

std::vector<BigInteger> RSA::block_bin_number_2_dec_number(std::vector<std::string> code_bin_number){
	std::vector<BigInteger> block_dec_number_return;
	int i;
	for(i=0;i<(int)code_bin_number.size();i++){
		block_dec_number_return.push_back(bin_number_2_dec_number(code_bin_number[i]));
	}
	return block_dec_number_return;
}

BigInteger RSA::bin_number_2_dec_number(std::string bin_number){
	BigInteger dec_number = 0;
	int length = bin_number.length();
	BigInteger teste_value = pow_BI(2,length-1);
	int i;
	for(i=0;i<length;i++){
		if(bin_number[i] == '1'){
			dec_number = dec_number + teste_value;
		}
		teste_value = teste_value/2;
	}
	return dec_number;
}

std::vector<BigInteger> RSA::block_dec_number_2_encrypt_data(std::vector<BigInteger> code_dec_number, BigInteger n, BigInteger e){
	std::vector<BigInteger> block_encrypt_data_return;
	int i;
	for(i=0;i<(int)code_dec_number.size();i++){
		block_encrypt_data_return.push_back(dec_number_2_encrypt_data(code_dec_number[i],n,e));
	}
	return block_encrypt_data_return;
}

BigInteger RSA::dec_number_2_encrypt_data(BigInteger dec_number, BigInteger n, BigInteger e){
	// return mod_BI(pow_BI(dec_number,e),n);
	return pow_BI_Mod(dec_number,e,n);
}

BigInteger RSA::mod_BI(BigInteger n, BigInteger a){
	return (n-((n/a)*a));
}

std::string RSA::read_file(std::string file_name){
	char ch;
	std::string input_message;
	FILE *arq;
	arq = fopen(file_name.c_str(),"r");
	if(arq == NULL){
		std::cout << "Error! Could not open file.\n";
	}else{
		while((ch=fgetc(arq))!=EOF){
			input_message = input_message + ch;
		}
	}
	fclose(arq);
	return input_message;
}

void RSA::write_cipher_in_file(std::string file_name, std::vector<BigInteger> block_encrypt_data){
	int i;
	std::string buffer;
	FILE *arq;
	arq = fopen(file_name.c_str(),"w");
	if(arq == NULL){
		std::cout << "Error! Could not open file.\n";
	}else{
		for(i=0;i<(int)block_encrypt_data.size()-1;i++){
			buffer = bigIntegerToString(block_encrypt_data[i]);
			fprintf(arq,"%s\n",buffer.c_str());
		}
		buffer = bigIntegerToString(block_encrypt_data[i]);
		fprintf(arq,"%s",buffer.c_str());
	}
	fclose(arq);
}

void RSA::write_message_decipher_in_file(std::string file_name, std::string message_decrypt){
	FILE *arq;
	arq = fopen(file_name.c_str(),"w");
	if(arq == NULL){
		std::cout << "Error! Could not open file.\n";
	}else{
		fprintf(arq,"%s",message_decrypt.c_str());
	}
	fclose(arq);
}

std::vector<BigInteger> RSA::read_file_cipher(std::string file_name){
	char ch;
	std::string input_message = "";
	std::vector<BigInteger> block_encrypt_data_return;
	FILE *arq;
	arq = fopen(file_name.c_str(),"r");
	if(arq == NULL){
		std::cout << "Error! Could not open file.\n";
	}else{
		while((ch=fgetc(arq))!=EOF){
			if(ch=='\n'){
				block_encrypt_data_return.push_back(stringToBigInteger(input_message));
				input_message = "";
			}else{
				input_message = input_message + ch;
			}
		}
		block_encrypt_data_return.push_back(stringToBigInteger(input_message));
	}
	fclose(arq);
	return block_encrypt_data_return;
}

void RSA::decrypt(std::string input, std::string output, BigInteger n, BigInteger d){
	std::vector<BigInteger> block_encrypt_data = read_file_cipher(input);
	std::vector<BigInteger> code_dec_number = block_dec_number_2_encrypt_data(block_encrypt_data,n,d);
	std::vector<std::string> code_bin_number = block_dec_number_2_bin_number(code_dec_number);
	std::vector<std::string> block_message = block_bin_number_2_parcer_message(code_bin_number);
	std::string message_decrypt = block_message_2_single_message(block_message);
	write_message_decipher_in_file(output,message_decrypt);
}

std::vector<std::string> RSA::block_dec_number_2_bin_number(std::vector<BigInteger> code_dec_number ){
	std::vector<std::string> code_bin_number_return;
	int i;
	for(i=0;i<(int)code_dec_number.size();i++){
		code_bin_number_return.push_back(dec_number_2_bin_number(code_dec_number[i]));
	}
	return code_bin_number_return;
}

std::vector<std::string> RSA::block_bin_number_2_parcer_message(std::vector<std::string> code_bin_number){
	std::vector<std::string> block_parcer_message_return;
	int i;
	for(i=0;i<(int)code_bin_number.size();i++){
		block_parcer_message_return.push_back(bin_number_2_parcer_message(code_bin_number[i]));
	}
	return block_parcer_message_return;
}

std::string RSA::bin_number_2_parcer_message(std::string bin_number){
	std::string parcer_message_return = "";
	int i;
	for(i=0;i<(int)bin_number.length()/8;i++){
		parcer_message_return = parcer_message_return + (char)(bin_number_2_dec_number(std::string(bin_number,i*8,8))).toInt();
	}
	return parcer_message_return;
}

std::string RSA::block_message_2_single_message(std::vector<std::string> block_message){
	std::string message_return = "";
	int i;
	for(i=0;i<(int)block_message.size();i++){
		message_return = message_return + block_message[i];
	}
	return message_return;
}

//The magic happend here
BigInteger RSA::brute_force_attack(BigInteger n, BigInteger e){
	return 0;
}