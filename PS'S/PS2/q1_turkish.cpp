#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(){
    // Rastgele sayı üretimi için zaman değerini tohum olarak kullanıyoruz.
    srand(time(0));
    
    // Toplam simülasyon sayısını belirleyen sabit değişken.
    const int simulationCount = 10000;
    
    // Her bir yarışmacının kazandığı düello sayısını tutan değişkenler.
    int winAaron = 0;
    int winBob = 0;
    int winCharlie = 0;
    
    // 10.000 düelloyu simüle ediyoruz.
    for(int i = 0; i < simulationCount; i++){
        // Her simülasyonda üç yarışmacının hayatta olup olmadığını belirleyen boolean değişkenler.
        bool aliveAaron = true;
        bool aliveBob = true;
        bool aliveCharlie = true;
        
        // Tek bir kişi hayatta kalana kadar düello devam eder.
        while((aliveAaron + aliveBob + aliveCharlie) > 1){
            // Aaron'un atışı: Eğer Aaron hâlâ hayattaysa.
            if(aliveAaron){
                // Öncelikle en iyi nişancı olan Charlie'yi hedef alır.
                if(aliveCharlie){
                    // Rastgele sayı 0 ile 1 arasında ve 1/3 ihtimalle Charlie vurulur.
                    if((rand() / (double)RAND_MAX) <= (1.0/3.0))
                        aliveCharlie = false;
                }
                // Eğer Charlie hayatta değilse, Bob üzerinde atış yapar.
                else if(aliveBob){
                    if((rand() / (double)RAND_MAX) <= (1.0/3.0))
                        aliveBob = false;
                }
            }
            
            // Düelloda yalnızca bir kişi kalmışsa döngüyü sonlandırırız.
            if((aliveAaron + aliveBob + aliveCharlie) == 1)
                break;
            
            // Bob'un atışı: Eğer Bob hâlâ hayattaysa.
            if(aliveBob){
                // En iyi nişancı olan Charlie hâlâ hayattaysa Charlie'ye atış yapar.
                if(aliveCharlie){
                    if((rand() / (double)RAND_MAX) <= 0.5)
                        aliveCharlie = false;
                }
                // Eğer Charlie hayatta değilse, Aaron'a atış yapar.
                else if(aliveAaron){
                    if((rand() / (double)RAND_MAX) <= 0.5)
                        aliveAaron = false;
                }
            }
            
            // Yine yalnızca bir kişinin kalıp kalmadığını kontrol ederiz.
            if((aliveAaron + aliveBob + aliveCharlie) == 1)
                break;
            
            // Charlie'nin atışı: Eğer Charlie hayattaysa.
            if(aliveCharlie){
                // Charlie, hiç isabet ettiremediği için rakiplerini kesin olarak öldürür.
                // Hayatta olan rakiplerden en iyi seçeneği belirleyip onu vurur.
                if(aliveBob){
                    aliveBob = false;
                } else if(aliveAaron){
                    aliveAaron = false;
                }
            }
        }
        
        // Sonunda düelloda hayatta kalan yarışmacının kazanma sayısını arttırıyoruz.
        if(aliveAaron)
            winAaron++;
        if(aliveBob)
            winBob++;
        if(aliveCharlie)
            winCharlie++;
    }
    
    // Toplam simülasyon sayısına göre her yarışmacının kazanma oranını hesaplıyor ve ekrana yazdırıyoruz.
    cout << "Aaron kazandi: " << winAaron << "/" << simulationCount << " (" 
         << (winAaron * 100.0 / simulationCount) << "%)" << endl;
    cout << "Bob kazandi: " << winBob << "/" << simulationCount << " (" 
         << (winBob * 100.0 / simulationCount) << "%)" << endl;
    cout << "Charlie kazandi: " << winCharlie << "/" << simulationCount << " (" 
         << (winCharlie * 100.0 / simulationCount) << "%)" << endl;
         
    return 0;
}