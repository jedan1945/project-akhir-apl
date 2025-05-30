#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

#define maks_user 10
#define maks_kamar 10
#define maks_review 10
#define maks_nama 50
#define RESET   "\033[0m"
#define MERAH   "\033[31m"
#define HIJAU   "\033[32m"
#define KUNING  "\033[33m"
#define CYAN    "\033[36m"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct User {
    string nama;
    string password;
    string role;
    bool has_pending_order;
    bool has_approved_room;
};

struct Kamar {
    int id;
    int harga;
    string jenis;
    string status;
    string penghuni;
    int kamar_tujuan;
};

struct Review {
    string nama_user;
    int rating;
    string komentar;
};

User data_user[maks_user];
Kamar data_kamar[maks_kamar];
Review data_review[maks_review];
int jumlah_user = 1;
int jumlah_kamar = 0;
int jumlah_review = 0;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool inputNumerik(int& nilai, const string& prompt, int min, int max) {
    string input;
    cout << KUNING << prompt << RESET;
    getline(cin, input);
    stringstream ss(input);
    if (ss >> nilai && ss.eof() && nilai >= min && nilai <= max) {
        return true;
    }
    cout << MERAH << "Input tidak valid. Masukkan angka antara " << min << " sampai " << max << "." << RESET << endl;
    system("pause");
    return false;
}

string formatUang(long long harga) {
    stringstream ss;
    ss << harga;
    string str = ss.str();
    string hasil;
    int hitung = 0;
    for (int i = str.length() - 1; i >= 0; i--) {
        hasil = str[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0) {
            hasil = '.' + hasil;
        }
    }
    return "Rp " + hasil;
}

void tampilkan_data_user() {
    clearScreen();
    cout << CYAN << "\n--- DAFTAR PENGHUNI KOST ---" << RESET << endl;
    if (jumlah_user <= 1) {
        cout << MERAH << "Belum ada penghuni terdaftar." << RESET << endl;
        system("pause");
        return;
    }
    cout << KUNING << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "Kamar" << setw(15) << "Status" << RESET << endl;
    cout << MERAH << "------------------------------------------------" << RESET << endl;
    
    for (int i = 1; i < jumlah_user; i++) {
        cout << left << setw(5) << i;
        cout << setw(20) << data_user[i].nama;
        bool punya_kamar = false;
        for (int j = 0; j < jumlah_kamar; j++) {
            if (data_kamar[j].penghuni == data_user[i].nama) {
                cout << setw(15) << data_kamar[j].id;
                cout << setw(15) << data_kamar[j].status;
                punya_kamar = true;
                break;
            }
        }
        if (!punya_kamar) {
            cout << HIJAU << setw(15) << "-";
            cout << setw(15) << "Tidak ada" << RESET;
        }
        cout << endl;
    }
    system("pause");
}

void tampilkan_data_kamar() {
    clearScreen();
    cout << CYAN << "\n--- DAFTAR KAMAR KOST ---" << RESET << endl;
    if (jumlah_kamar == 0) {
        cout << MERAH << "Belum ada kamar tersedia." << RESET << endl;
        return;
    }
    cout << KUNING << left << setw(5) << "No" << setw(20) << "Jenis" << setw(15) << "Harga" 
        << setw(15) << "Status" << setw(15) << "Penghuni" << RESET << endl;
    cout << MERAH << "---------------------------------------------------------------" << RESET << endl;
    
    for (int i = 0; i < jumlah_kamar; i++) {
        cout << left << setw(5) << data_kamar[i].id;
        cout << setw(20) << data_kamar[i].jenis;
        cout << setw(15) << formatUang(data_kamar[i].harga);
        cout << setw(15) << data_kamar[i].status;
        cout << setw(15) << (data_kamar[i].penghuni == "-" ? "Tidak ada" : data_kamar[i].penghuni) << endl;
    }
}

bool validasi_nama(const string& nama) {
    if (nama.empty()) {
        cout << MERAH << "Nama tidak boleh kosong." << RESET << endl;
        system("pause");
        return false;
    }
    if (nama.length() > maks_nama) {
        cout << MERAH << "Nama terlalu panjang (maks " << maks_nama << " karakter)." << RESET << endl;
        system("pause");
        return false;
    }
    if (nama == "admin") {
        cout << MERAH << "Nama 'admin' tidak boleh digunakan!" << RESET << endl;
        system("pause");
        return false;
    }
    for (int i = 0; i < jumlah_user; i++) {
        if (data_user[i].nama == nama) {
            cout << MERAH << "Nama sudah digunakan!" << RESET << endl;
            system("pause");
            return false;
        }
    }
    return true;
}

void registrasi() {
    clearScreen();
    string nama, sandi;
    int percobaan = 0;
    cout << CYAN << "--- REGISTRASI PENGGUNA BARU ---" << RESET << endl;
    while (percobaan < 3) {
        cout << KUNING << "Masukkan nama (0 untuk kembali): " << RESET;
        getline(cin, nama);
        if (nama == "0") return;
        cout << KUNING << "Masukkan password: " << RESET;
        getline(cin, sandi);
        if (sandi.empty()) {
            cout << MERAH << "Password tidak boleh kosong." << RESET << endl;
            system("pause");
            percobaan++;
            continue;
        }
        if (sandi.length() > maks_nama) {
            cout << MERAH << "Password terlalu panjang (maks " << maks_nama << " karakter)." << RESET << endl;
            system("pause");
            percobaan++;
            continue;
        }
        if (!validasi_nama(nama)) {
            percobaan++;
            continue;
        }
        if (jumlah_user >= maks_user) {
            cout << MERAH << "Kapasitas pengguna penuh!" << RESET << endl;
            system("pause");
            return;
        }
        data_user[jumlah_user] = {nama, sandi, "user", false, false};
        jumlah_user++;
        cout << HIJAU << "\nRegistrasi berhasil! Silakan login." << RESET << endl;
        system("pause");
        return;
    }
    cout << MERAH << "Gagal registrasi setelah 3 kali percobaan." << RESET << endl;
    system("pause");
}

bool login(int &data_login) {
    clearScreen();
    string nama, sandi;
    int percobaan = 0;
    while (percobaan < 3) {
        cout << CYAN << "--- LOGIN ---" << RESET << endl;
        cout << KUNING << "Masukkan nama (0 untuk kembali): " << RESET;
        getline(cin, nama);
        if (nama == "0") return false;
        cout << KUNING << "Masukkan password: " << RESET;
        getline(cin, sandi);
        if (sandi.empty()) {
            cout << MERAH << "Password tidak boleh kosong." << RESET << endl;
            system("pause");
            percobaan++;
            continue;
        }
        for (int i = 0; i < jumlah_user; i++) {
            if (data_user[i].nama == nama && data_user[i].password == sandi) {
                data_login = i;
                return true;
            }
        }
        percobaan++;
        cout << MERAH << "Nama atau password salah. Percobaan " << percobaan << "/3" << RESET << endl;
        system("pause");
        clearScreen();
    }
    cout << MERAH << "Melebihi batas percobaan login." << RESET << endl;
    system("pause");
    exit(0);
}

void lihat_kamar_terurut(int sort_dengan) {
    clearScreen();
    if (jumlah_kamar == 0) {
        cout << MERAH << "Belum ada kamar tersedia." << RESET << endl;
        system("pause");
        return;
    }
    vector<Kamar> kamar_list(data_kamar, data_kamar + jumlah_kamar);
    if (sort_dengan == 1) {
        sort(kamar_list.begin(), kamar_list.end(), [](const Kamar &a, const Kamar &b) {
            return a.harga < b.harga;
        });
    } else if (sort_dengan == 2) {
        sort(kamar_list.begin(), kamar_list.end(), [](const Kamar &a, const Kamar &b) {
            return a.jenis < b.jenis;
        });
    } else if (sort_dengan == 3) {
        sort(kamar_list.begin(), kamar_list.end(), [](const Kamar &a, const Kamar &b) {
            return a.status < b.status;
        });
    } else {
        cout << MERAH << "Pilihan tidak valid." << RESET << endl;
        system("pause");
        return;
    }
    cout << CYAN << "\n--- Data Kamar (Terurut) ---" << RESET << endl;
    for (const auto &kamar : kamar_list) {
        cout << KUNING << "Kamar " << kamar.id << endl;
        cout << "Harga: " << formatUang(kamar.harga) << endl;
        cout << "Jenis: " << kamar.jenis << endl;
        cout << "Status: " << kamar.status << endl;
        cout << "Penghuni: " << (kamar.penghuni == "-" ? "Tidak ada" : kamar.penghuni) << RESET << endl;
        bool ada_review = false;
        for (int i = 0; i < jumlah_review; i++) {
            if (data_review[i].nama_user == kamar.penghuni) {
                if (!ada_review) {
                    cout << "Review:" << endl;
                    ada_review = true;
                }
                cout << "  - Rating: " << data_review[i].rating << "/5, Komentar: " 
                    << data_review[i].komentar << endl;
            }
        }
        cout << endl;
    }
    system("pause");
}

void cari_kamar(int search_by) {
    clearScreen();
    if (jumlah_kamar == 0) {
        cout << MERAH << "Belum ada kamar tersedia." << RESET << endl;
        system("pause");
        return;
    }
    string keyword;
    int harga_keyword;
    bool found = false;
    cout << CYAN << "\n--- Pencarian Kamar ---" << RESET << endl;
    if (search_by == 1) {
        cout << KUNING << "Masukkan jenis kamar: " << RESET;
        getline(cin, keyword);
        if (keyword.empty()) {
            cout << MERAH << "Jenis kamar tidak boleh kosong." << RESET << endl;
            system("pause");
            return;
        }
        for (int i = 0; i < jumlah_kamar; i++) {
            if (data_kamar[i].jenis.find(keyword) != string::npos) {
                cout << "Kamar " << data_kamar[i].id << endl;
                cout << "Harga: " << formatUang(data_kamar[i].harga) << endl;
                cout << "Jenis: " << data_kamar[i].jenis << endl;
                cout << "Status: " << data_kamar[i].status << endl;
                cout << "Penghuni: " << (data_kamar[i].penghuni == "-" ? "Tidak ada" : data_kamar[i].penghuni) << endl;
                bool ada_review = false;
                for (int j = 0; j < jumlah_review; j++) {
                    if (data_review[j].nama_user == data_kamar[i].penghuni) {
                        if (!ada_review) {
                            cout << "Review:" << endl;
                            ada_review = true;
                        }
                        cout << "  - Rating: " << data_review[j].rating << "/5, Komentar: " 
                            << data_review[j].komentar << endl;
                    }
                }
                cout << endl;
                found = true;
            }
        }
    } else if (search_by == 2) {
        cout << KUNING << "Masukkan nama penghuni: " << RESET;
        getline(cin, keyword);
        if (keyword.empty()) {
            cout << MERAH << "Nama penghuni tidak boleh kosong." << RESET << endl;
            system("pause");
            return;
        }
        for (int i = 0; i < jumlah_kamar; i++) {
            if (data_kamar[i].penghuni.find(keyword) != string::npos) {
                cout << "Kamar " << data_kamar[i].id << endl;
                cout << "Harga: " << formatUang(data_kamar[i].harga) << endl;
                cout << "Jenis: " << data_kamar[i].jenis << endl;
                cout << "Status: " << data_kamar[i].status << endl;
                cout << "Penghuni: " << (data_kamar[i].penghuni == "-" ? "Tidak ada" : data_kamar[i].penghuni) << endl;
                bool ada_review = false;
                for (int j = 0; j < jumlah_review; j++) {
                    if (data_review[j].nama_user == data_kamar[i].penghuni) {
                        if (!ada_review) {
                            cout << "Review:" << endl;
                            ada_review = true;
                        }
                        cout << "  - Rating: " << data_review[j].rating << "/5, Komentar: " 
                            << data_review[j].komentar << endl;
                    }
                }
                cout << endl;
                found = true;
            }
        }
    } else if (search_by == 3) {
        while (!inputNumerik(harga_keyword, "Masukkan harga maksimal: ", 0, INT_MAX)) {
            clearScreen();
            cout << CYAN << "\n--- Pencarian Kamar ---" << RESET << endl;
        }
        for (int i = 0; i < jumlah_kamar; i++) {
            if (data_kamar[i].harga <= harga_keyword) {
                cout << "Kamar " << data_kamar[i].id << endl;
                cout << "Harga: " << formatUang(data_kamar[i].harga) << endl;
                cout << "Jenis: " << data_kamar[i].jenis << endl;
                cout << "Status: " << data_kamar[i].status << endl;
                cout << "Penghuni: " << (data_kamar[i].penghuni == "-" ? "Tidak ada" : data_kamar[i].penghuni) << endl;
                bool ada_review = false;
                for (int j = 0; j < jumlah_review; j++) {
                    if (data_review[j].nama_user == data_kamar[i].penghuni) {
                        if (!ada_review) {
                            cout << "Review:" << endl;
                            ada_review = true;
                        }
                        cout << "  - Rating: " << data_review[j].rating << "/5, Komentar: " 
                            << data_review[j].komentar << endl;
                    }
                }
                cout << endl;
                found = true;
            }
        }
    } else {
        cout << MERAH << "Pilihan tidak valid." << RESET << endl;
        system("pause");
        return;
    }
    if (!found) {
        cout << MERAH << "Tidak ditemukan kamar dengan kriteria tersebut." << RESET << endl;
    }
    system("pause");
}

void menu_admin() {
    int pilihan;
    do {
        clearScreen();
        cout << CYAN << "=== MENU ADMIN ===" << RESET << endl;
        cout << "1. Kelola Data Kamar" << endl;
        cout << "2. Kelola Data Penghuni" << endl;
        cout << "3. Kelola Pemesanan" << endl;
        cout << "4. Keluar" << endl;
        while (!inputNumerik(pilihan, "Pilih menu: ", 1, 4)) {
            clearScreen();
            menu_admin();
        }
        switch (pilihan) {
            case 1: {
                int submenu;
                do {
                    clearScreen();
                    cout << CYAN << "--- KELOLA DATA KAMAR ---" << RESET << endl;
                    tampilkan_data_kamar();
                    cout << "\n1. Tambah Kamar" << endl;
                    cout << "2. Edit Kamar" << endl;
                    cout << "3. Hapus Kamar" << endl;
                    cout << "4. Kembali" << endl;
                    while (!inputNumerik(submenu, "Pilih: ", 1, 4)) {
                        clearScreen();
                        break;
                    }
                    if (submenu == 1) {
                        if (jumlah_kamar >= maks_kamar) {
                            cout << MERAH << "Kapasitas kamar penuh!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        Kamar kamar_baru;
                        kamar_baru.id = jumlah_kamar + 1;
                        cout << KUNING << "Masukkan jenis kamar: " << RESET;
                        getline(cin, kamar_baru.jenis);
                        if (kamar_baru.jenis.empty()) {
                            cout << MERAH << "Jenis kamar tidak boleh kosong." << RESET << endl;
                            system("pause");
                            continue;
                        }
                        if (kamar_baru.jenis.length() > maks_nama) {
                            cout << MERAH << "Jenis kamar terlalu panjang (maks " << maks_nama << " karakter)." << RESET << endl;
                            system("pause");
                            continue;
                        }
                        while (!inputNumerik(kamar_baru.harga, "Masukkan harga kamar: ", 0, INT_MAX)) {
                            clearScreen();
                            cout << CYAN << "--- KELOLA DATA KAMAR ---" << RESET << endl;
                        }
                        kamar_baru.status = "kosong";
                        kamar_baru.penghuni = "-";
                        kamar_baru.kamar_tujuan = 0;
                        data_kamar[jumlah_kamar++] = kamar_baru;
                        cout << HIJAU << "Kamar berhasil ditambahkan!" << RESET << endl;
                        system("pause");
                    } else if (submenu == 2) {
                        if (jumlah_kamar == 0) {
                            cout << MERAH << "Belum ada kamar tersedia!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        int no_kamar;
                        while (!inputNumerik(no_kamar, "Masukkan nomor kamar yang akan diedit (0 untuk batal): ", 0, jumlah_kamar)) {
                            clearScreen();
                            cout << CYAN << "--- KELOLA DATA KAMAR ---" << RESET << endl;
                            tampilkan_data_kamar();
                        }
                        if (no_kamar == 0) continue;
                        Kamar& kamar = data_kamar[no_kamar-1];
                        cout << KUNING << "Data kamar saat ini:" << RESET << endl;
                        cout << "Jenis: " << kamar.jenis << endl;
                        cout << "Harga: " << formatUang(kamar.harga) << endl;
                        cout << KUNING << "Masukkan jenis baru: " << RESET;
                        getline(cin, kamar.jenis);
                        if (kamar.jenis.empty()) {
                            cout << MERAH << "Jenis kamar tidak boleh kosong." << RESET << endl;
                            system("pause");
                            continue;
                        }
                        if (kamar.jenis.length() > maks_nama) {
                            cout << MERAH << "Jenis kamar terlalu panjang." << RESET << endl;
                            system("pause");
                            continue;
                        }
                        while (!inputNumerik(kamar.harga, "Masukkan harga baru: ", 0, INT_MAX)) {
                            clearScreen();
                            cout << CYAN << "--- KELOLA DATA KAMAR ---" << RESET << endl;
                        }
                        cout << HIJAU << "Data kamar berhasil diperbarui!" << RESET << endl;
                        system("pause");
                    } else if (submenu == 3) {
                        if (jumlah_kamar == 0) {
                            cout << MERAH << "Belum ada kamar tersedia!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        int no_kamar;
                        while (!inputNumerik(no_kamar, "Masukkan nomor kamar yang akan dihapus (0 untuk batal): ", 0, jumlah_kamar)) {
                            clearScreen();
                            cout << CYAN << "--- KELOLA DATA KAMAR ---" << RESET << endl;
                            tampilkan_data_kamar();
                        }
                        if (no_kamar == 0) continue;
                        if (data_kamar[no_kamar-1].status != "kosong") {
                            cout << MERAH << "Kamar tidak kosong, tidak dapat dihapus!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        for (int i = no_kamar-1; i < jumlah_kamar-1; i++) {
                            data_kamar[i] = data_kamar[i+1];
                            data_kamar[i].id = i + 1;
                        }
                        jumlah_kamar--;
                        cout << HIJAU << "Kamar berhasil dihapus!" << RESET << endl;
                        system("pause");
                    }
                } while (submenu != 4);
                break;
            }
            case 2: {
                int submenu;
                do {
                    clearScreen();
                    cout << CYAN << "--- KELOLA DATA PENGHUNI ---" << RESET << endl;
                    tampilkan_data_user();
                    cout << "\n1. Edit Data Penghuni" << endl;
                    cout << "2. Hapus Penghuni" << endl;
                    cout << "3. Kembali" << endl;
                    while (!inputNumerik(submenu, "Pilih: ", 1, 3)) {
                        clearScreen();
                        cout << CYAN << "--- KELOLA DATA PENGHUNI ---" << RESET << endl;
                        tampilkan_data_user();
                    }
                    if (submenu == 1) {
                        if (jumlah_user <= 1) {
                            cout << MERAH << "Belum ada penghuni terdaftar!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        string nama;
                        cout << KUNING << "Masukkan nama penghuni yang akan diedit (0 untuk batal): " << RESET;
                        getline(cin, nama);
                        if (nama == "0") continue;
                        if (nama.empty()) {
                            cout << MERAH << "Nama tidak boleh kosong." << RESET << endl;
                            system("pause");
                            continue;
                        }
                        int index = -1;
                        for (int i = 1; i < jumlah_user; i++) {
                            if (data_user[i].nama == nama) {
                                index = i;
                                break;
                            }
                        }
                        if (index == -1) {
                            cout << MERAH << "Penghuni tidak ditemukan!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        string nama_baru;
                        while (true) {
                            cout << KUNING << "Masukkan nama baru (0 untuk batal): " << RESET;
                            getline(cin, nama_baru);
                            if (nama_baru == "0") break;
                            if (nama_baru == nama) {
                                cout << MERAH << "Nama sama dengan sebelumnya!" << RESET << endl;
                                system("pause");
                                continue;
                            }
                            if (validasi_nama(nama_baru)) break;
                        }
                        if (nama_baru != "0") {
                            data_user[index].nama = nama_baru;
                            for (int i = 0; i < jumlah_kamar; i++) {
                                if (data_kamar[i].penghuni == nama) {
                                    data_kamar[i].penghuni = nama_baru;
                                }
                            }
                            for (int i = 0; i < jumlah_review; i++) {
                                if (data_review[i].nama_user == nama) {
                                    data_review[i].nama_user = nama_baru;
                                }
                            }
                            cout << KUNING << "Masukkan password baru: " << RESET;
                            getline(cin, data_user[index].password);
                            if (data_user[index].password.empty()) {
                                cout << MERAH << "Password tidak boleh kosong." << RESET << endl;
                                system("pause");
                                continue;
                            }
                            if (data_user[index].password.length() > maks_nama) {
                                cout << MERAH << "Password terlalu panjang." << RESET << endl;
                                system("pause");
                                continue;
                            }
                            cout << HIJAU << "Data penghuni berhasil diperbarui!" << RESET << endl;
                            system("pause");
                        }
                    } else if (submenu == 2) {
                        if (jumlah_user <= 1) {
                            cout << MERAH << "Belum ada penghuni terdaftar!" << RESET << endl;
                            system("pause");
                            continue;
                        }
                        string nama_d;
                        cout << KUNING << "Masukkan nama penghuni yang akan dihapus (0 untuk batal): " << RESET;
                        getline(cin, nama_d);
                        if (nama_d == "0") continue;
                        if (nama_d.empty()) {
                            cout << MERAH << "Nama tidak boleh kosong." << endl;
                            system("pause");
                            continue;
                        }
                        int index = -1;
                        for (int i = 0; i < jumlah_user; i++) {
                            if (data_user[i].nama == nama_d) {
                                index = i;
                                break;
                            }
                        }
                        if (index == -1) {
                            cout << MERAH << "Penghuni tidak ditemukan!" << endl;
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < jumlah_kamar; i++) {
                            if (data_kamar[i].penghuni == nama_d) {
                                data_kamar[i].penghuni = "-";
                                data_kamar[i].status = "kosong";
                                data_kamar[i].kamar_tujuan = 0;
                            }
                        }
                        for (int i = 0; i < jumlah_review; i++) {
                            if (data_review[i].nama_user == nama_d) {
                                for (int j = i; j < jumlah_review-1; j++) {
                                    data_review[j] = data_review[j+1];
                                }
                                jumlah_review--;
                                i--;
                            }
                        }
                        for (int i = index; i < jumlah_user-1; i++) {
                            data_user[i] = data_user[i+1];
                        }
                        jumlah_user--;

                        cout << HIJAU << "Penghuni berhasil dihapus!" << endl;
                        system("pause");
                    }
                } while (submenu != 3);
                break;
            }
            
            case 3: {
                int submenu;
                do {
                    clearScreen();
                    cout << CYAN << "\n--- KELOLA PEMESANAN ---" << RESET << endl;
                    cout << "1. Setujui Pemesanan Kamar" << endl;
                    cout << "2. Setujui Permintaan Pindah Kamar" << endl;
                    cout << "3. Kembali" << endl;
                    while (!inputNumerik(submenu, "Pilih: ", 1, 3)) {
                        clearScreen();
                        cout << CYAN << "\n--- KELOLA PEMESANAN ---" << RESET << endl;
                        cout << "1. Setujui Pemesanan Kamar" << endl;
                        cout << "2. Setujui Permintaan Pindah Kamar" << endl;
                        cout << "3. Kembali" << endl;
                    }
                        if (submenu == 1) {
                            clearScreen();
                            bool ada_pesanan = false;
                            for (int i = 0; i < jumlah_kamar; i++) {
                                if (data_kamar[i].status == "dipesan") {
                                    ada_pesanan = true;
                                    cout << "\n"<< "Kamar " << data_kamar[i].id << ": " << data_kamar[i].jenis 
                                        << "\nHarga: " << formatUang(data_kamar[i].harga) 
                                        << "\nPemesan: " << data_kamar[i].penghuni << endl;

                                    char konfirmasi;
                                    bool valid = false;
                                    while (!valid) {
                                        cout << KUNING << "Setujui pemesanan ini? (y/n): " << RESET;
                                        string input;
                                        getline(cin, input);
                                        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' || input[0] == 'n' || input[0] == 'N')) {
                                            konfirmasi = input[0];
                                            valid = true;
                                        } else {
                                            cout << MERAH << "Input hanya boleh 'y' atau 'n'." << RESET << endl;
                                            system("pause");
                                        }
                                    }

                                    if (konfirmasi == 'y' || konfirmasi == 'Y') {
                                        data_kamar[i].status = "terisi";
                                        for (int j = 0; j < jumlah_user; j++) {
                                            if (data_user[j].nama == data_kamar[i].penghuni) {
                                                data_user[j].has_pending_order = false;
                                                data_user[j].has_approved_room = true;
                                                break;
                                            }
                                        }
                                        cout << HIJAU << "Pesanan disetujui!" << RESET << endl;
                                        system("pause");
                                    } else {
                                        data_kamar[i].status = "kosong";
                                        data_kamar[i].penghuni = "-";
                                        data_kamar[i].kamar_tujuan = 0;
                                        for (int j = 0; j < jumlah_user; j++) {
                                            if (data_user[j].nama == data_kamar[i].penghuni) {
                                                data_user[j].has_pending_order = false;
                                                break;
                                            }
                                        }
                                        cout << MERAH << "Pesanan ditolak!" << endl;
                                        system("pause");
                                    }
                                }
                            }
                            if (!ada_pesanan) {
                                cout << MERAH << "Tidak ada pesanan yang menunggu konfirmasi!" << endl;
                                system("pause");
                            }
                        } else if (submenu == 2) {
                            clearScreen();
                            bool ada_pindah = false;
                            for (int i = 0; i < jumlah_kamar; i++) {
                                if (data_kamar[i].status == "minta_pindah" && data_kamar[i].kamar_tujuan > 0 && data_kamar[i].kamar_tujuan <= jumlah_kamar) {
                                    ada_pindah = true;
                                    int tujuan = data_kamar[i].kamar_tujuan;
                                    if (data_kamar[tujuan-1].status == "kosong") {
                                        cout << "\nPermintaan pindah dari: " << data_kamar[i].penghuni 
                                            << "\nKamar saat ini: " << data_kamar[i].id 
                                            << " (" << data_kamar[i].jenis << ", Harga: " << formatUang(data_kamar[i].harga) << ")"
                                            << "\nKamar tujuan: " << tujuan 
                                            << " (" << data_kamar[tujuan-1].jenis << ", Harga: " << formatUang(data_kamar[tujuan-1].harga) << ")" << endl;

                                        char konfirmasi;
                                        bool valid = false;
                                        while (!valid) {
                                            cout << KUNING << "Setujui pindah ke kamar " << tujuan << "? (y/n): " << RESET;
                                            string input;
                                            getline(cin, input);
                                            if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' || input[0] == 'n' || input[0] == 'N')) {
                                                konfirmasi = input[0];
                                                valid = true;
                                            } else {
                                                cout << MERAH << "Input hanya boleh 'y' atau 'n'." << RESET << endl;
                                                system("pause");
                                            }
                                        }
                                        if (konfirmasi == 'y' || konfirmasi == 'Y') {
                                            data_kamar[tujuan-1].penghuni = data_kamar[i].penghuni;
                                            data_kamar[tujuan-1].status = "terisi";
                                            data_kamar[tujuan-1].kamar_tujuan = 0;
                                            data_kamar[i].penghuni = "-";
                                            data_kamar[i].status = "kosong";
                                            data_kamar[i].kamar_tujuan = 0;
                                            cout << HIJAU << "Pindah kamar disetujui!" << RESET << endl;
                                            system("pause");
                                        } else {
                                            data_kamar[i].status = "terisi";
                                            data_kamar[i].kamar_tujuan = 0;
                                            cout << MERAH << "Pindah kamar ditolak!" << RESET << endl;
                                            system("pause");
                                        }
                                    } else {
                                        cout << MERAH << "\nKamar tujuan " << tujuan << " sudah tidak tersedia (status: " << data_kamar[tujuan-1].status << "). Permintaan dibatalkan." << RESET << endl;
                                        data_kamar[i].status = "terisi";
                                        data_kamar[i].kamar_tujuan = 0;
                                        system("pause");
                                    }
                                }
                            }
                            if (!ada_pindah) {
                                cout << MERAH << "Tidak ada permintaan pindah kamar!" << RESET << endl;
                                system("pause");
                            }
                        }
                    } while (submenu != 3);
                    break;
                }
        }
    } while (pilihan != 4);
}

void menu_user(int id_user) {
    int pilihan;
    do {
        clearScreen();
        cout << CYAN << "=== MENU USER ===" << RESET << endl;
        cout << "Status Konfirmasi: ";
        bool has_pending = false;
        bool has_approved = false;
        for (int i = 0; i < jumlah_kamar; i++) {
            if (data_kamar[i].penghuni == data_user[id_user].nama) {
                if (data_kamar[i].status == "dipesan") {
                    has_pending = true;
                    cout << "Pesanan kamar " << data_kamar[i].id << " menunggu konfirmasi." << endl;
                } else if (data_kamar[i].status == "terisi") {
                    has_approved = true;
                    cout << "Kamar " << data_kamar[i].id << " telah disetujui." << endl;
                } else if (data_kamar[i].status == "minta_pindah") {
                    has_approved = true;
                    cout << "Permintaan pindah dari kamar " << data_kamar[i].id << " ke kamar " 
                        << data_kamar[i].kamar_tujuan << " menunggu konfirmasi." << endl;
                }
            }
        }
        if (!has_pending && !has_approved) {
            cout << "Tidak ada pesanan." << endl;
        }
        cout << "\n1. Pesan kamar" << endl;
        cout << "2. Menu Kamar" << endl;
        cout << "3. Menu Review" << endl;
        cout << "4. Kembali" << endl;
        while (!inputNumerik(pilihan, "Pilih: ", 1, 4)) {
            clearScreen();
            cout << CYAN << "=== MENU USER ---" << RESET << endl;
            cout << "Status: ";
            if (has_pending) cout << "Pesanan menunggu konfirmasi." << endl;
            if (has_approved) cout << "Kamar disetujui atau pindah menunggu konfirmasi." << endl;
            if (!has_pending && !has_approved) cout << "Tidak ada pesanan." << endl;
            cout << "\n1. Pesan kamar" << endl;
            cout << "2. Menu Kamar" << endl;
            cout << "3. Menu Review" << endl;
            cout << "4. Kembali" << endl;
        }
        if (pilihan == 1) {
            clearScreen();
            if (data_user[id_user].has_approved_room) {
                cout << MERAH << "Anda sudah memiliki kamar yang disetujui!" << RESET << endl;
                system("pause");
                continue;
            }
            if (data_user[id_user].has_pending_order) {
                cout << MERAH << "Anda memiliki pesanan menunggu konfirmasi!" << RESET << endl;
                system("pause");
                continue;
            }
            bool ada_kosong = false;
            cout << "\n--- Daftar Kamar Kosong ---" << endl;
            for (int i = 0; i < jumlah_kamar; i++) {
                if (data_kamar[i].status == "kosong") {
                    cout << "Kamar " << data_kamar[i].id << ": Harga: " << formatUang(data_kamar[i].harga) 
                        << ", Jenis: " << data_kamar[i].jenis << endl;
                    ada_kosong = true;
                }
            }
            if (!ada_kosong) {
                cout << MERAH << "Tidak ada kamar kosong!" << RESET << endl;
                system("pause");
                continue;
            }
            int no_kamar;
            while (!inputNumerik(no_kamar, "Masukkan nomor kamar (0 untuk batal): ", 0, jumlah_kamar)) {
                clearScreen();
                cout << "\n--- Daftar Kamar Kosong ---" << endl;
                for (int i = 0; i < jumlah_kamar; i++) {
                    if (data_kamar[i].status == "kosong") {
                        cout << "Kamar " << data_kamar[i].id << ": Harga: " << formatUang(data_kamar[i].harga) 
                            << ", Jenis: " << data_kamar[i].jenis << endl;
                    }
                }
            }
            if (no_kamar == 0) continue;
            if (data_kamar[no_kamar-1].status != "kosong") {
                cout << MERAH << "Kamar tidak tersedia!" << RESET << endl;
                system("pause");
                continue;
            }
            data_kamar[no_kamar-1].status = "dipesan";
            data_kamar[no_kamar-1].penghuni = data_user[id_user].nama;
            data_kamar[no_kamar-1].kamar_tujuan = 0;
            data_user[id_user].has_pending_order = true;
            cout << HIJAU << "Kamar berhasil dipesan! Menunggu konfirmasi admin." << RESET << endl;
            system("pause");
        } else if (pilihan == 2) {
            int menu_kamar;
            do {
                clearScreen();
                cout << CYAN << "\n--- MENU KAMAR ---" << RESET << endl;
                cout << "1. Pindah kamar" << endl;
                cout << "2. Lihat kamar" << endl;
                cout << "3. Kembali" << endl;
                while (!inputNumerik(menu_kamar, "Pilih: ", 1, 3)) {
                    clearScreen();
                    cout << CYAN << "\n--- MENU KAMAR ---" << RESET << endl;
                    cout << "1. Pindah kamar" << endl;
                    cout << "2. Lihat kamar" << endl;
                    cout << "3. Kembali" << endl;
                }
                if (menu_kamar == 1) {
                    clearScreen();
                    bool has_room = false;
                    int current_room = -1;
                    for (int i = 0; i < jumlah_kamar; i++) {
                        if (data_kamar[i].penghuni == data_user[id_user].nama && 
                            data_kamar[i].status == "terisi") {
                            current_room = i;
                            has_room = true;
                            break;
                        }
                    }
                    if (!has_room) {
                        cout << MERAH << "Anda belum memiliki kamar!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    bool ada_kosong = false;
                    cout << "\n--- Daftar Kamar Tersedia ---" << endl;
                    for (int i = 0; i < jumlah_kamar; i++) {
                        if (data_kamar[i].status == "kosong") {
                            cout << "Kamar " << data_kamar[i].id << ": Harga: " << formatUang(data_kamar[i].harga) 
                                << ", Jenis: " << data_kamar[i].jenis << endl;
                            ada_kosong = true;
                        }
                    }
                    if (!ada_kosong) {
                        cout << MERAH << "Tidak ada kamar tersedia untuk pindah!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    int kamar_tujuan;
                    while (!inputNumerik(kamar_tujuan, "Masukkan nomor kamar tujuan (0 untuk batal): ", 0, jumlah_kamar)) {
                        clearScreen();
                        cout << "\n--- Daftar Kamar Tersedia ---" << endl;
                        for (int i = 0; i < jumlah_kamar; i++) {
                            if (data_kamar[i].status == "kosong") {
                                cout << "Kamar " << data_kamar[i].id << ": Harga: " << formatUang(data_kamar[i].harga) 
                                    << ", Jenis: " << data_kamar[i].jenis << endl;
                            }
                        }
                    }
                    if (kamar_tujuan == 0) continue;
                    if (kamar_tujuan == data_kamar[current_room].id) {
                        cout << MERAH << "Anda tidak dapat pindah ke kamar yang sama!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    if (data_kamar[kamar_tujuan-1].status != "kosong") {
                        cout << MERAH << "Kamar tidak tersedia (status: " << data_kamar[kamar_tujuan-1].status << ")!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    char konfirmasi;
                    bool valid = false;
                    while (!valid) {
                        cout << KUNING << "Yakin ingin pindah ke kamar " << kamar_tujuan << "? (y/n): " << RESET;
                        string input;
                        getline(cin, input);
                        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' || input[0] == 'n' || input[0] == 'N')) {
                            konfirmasi = input[0];
                            valid = true;
                        } else {
                            cout << MERAH << "Input hanya boleh 'y' atau 'n'." << RESET << endl;
                            system("pause");
                        }
                    }
                    if (konfirmasi == 'y' || konfirmasi == 'Y') {
                        data_kamar[current_room].status = "minta_pindah";
                        data_kamar[current_room].kamar_tujuan = kamar_tujuan;
                        cout << HIJAU << "Permintaan pindah kamar dikirim! Menunggu konfirmasi admin." << RESET << endl;
                        system("pause");
                    }
                } else if (menu_kamar == 2) {
                    int lihat_kamar;
                    do {
                        clearScreen();
                        cout << CYAN << "\n--- LIHAT KAMAR ---" << RESET << endl;
                        cout << "1. Lihat semua kamar" << endl;
                        cout << "2. Sorting kamar" << endl;
                        cout << "3. Searching kamar" << endl;
                        cout << "4. Kembali" << endl;
                        while (!inputNumerik(lihat_kamar, "Pilih: ", 1, 4)) {
                            clearScreen();
                            cout << CYAN << "\n--- LIHAT KAMAR ---" << RESET << endl;
                            cout << "1. Lihat semua kamar" << endl;
                            cout << "2. Sorting kamar" << endl;
                            cout << "3. Searching kamar" << endl;
                            cout << "4. Kembali" << endl;
                        }
                        if (lihat_kamar == 1) {
                            tampilkan_data_kamar();
                        } else if (lihat_kamar == 2) {
                            int sort_by;
                            do {
                                clearScreen();
                                cout << CYAN << "\n--- SORTING KAMAR ---" << RESET << endl;
                                cout << "1. Sort by harga" << endl;
                                cout << "2. Sort by jenis" << endl;
                                cout << "3. Sort by status" << endl;
                                cout << "4. Kembali" << endl;
                                while (!inputNumerik(sort_by, "Pilih: ", 1, 4)) {
                                    clearScreen();
                                    cout << CYAN << "\n--- SORTING KAMAR ---" << RESET << endl;
                                    cout << "1. Sort by harga" << endl;
                                    cout << "2. Sort by jenis" << endl;
                                    cout << "3. Sort by status" << endl;
                                    cout << "4. Kembali" << endl;
                                }
                                if (sort_by >= 1 && sort_by <= 3) {
                                    lihat_kamar_terurut(sort_by);
                                }
                            } while (sort_by != 4);
                        } else if (lihat_kamar == 3) {
                            int search_by;
                            do {
                                clearScreen();
                                cout << CYAN << "\n--- SEARCHING KAMAR ---" << RESET << endl;
                                cout << "1. Search by jenis" << endl;
                                cout << "2. Search by penghuni" << endl;
                                cout << "3. Search by harga" << endl;
                                cout << "4. Kembali" << endl;
                                while (!inputNumerik(search_by, "Pilih: ", 1, 4)) {
                                    clearScreen();
                                    cout << CYAN << "\n--- SEARCHING KAMAR ---" << RESET << endl;
                                    cout << "1. Search by jenis" << endl;
                                    cout << "2. Search by penghuni" << endl;
                                    cout << "3. Search by harga" << endl;
                                    cout << "4. Kembali" << endl;
                                }
                                if (search_by >= 1 && search_by <= 3) {
                                    cari_kamar(search_by);
                                }
                            } while (search_by != 4);
                        }
                    } while (lihat_kamar != 4);
                }
            } while (menu_kamar != 3);
        } else if (pilihan == 3) {
            int menu_review;
            do {
                clearScreen();
                cout << CYAN << "\n--- MENU REVIEW ---" << RESET << endl;
                cout << "1. Beri rating" << endl;
                cout << "2. Hapus rating" << endl;
                cout << "3. Kembali" << endl;
                while (!inputNumerik(menu_review, "Pilih: ", 1, 3)) {
                    clearScreen();
                    cout << CYAN << "\n--- MENU REVIEW ---" << RESET << endl;
                    cout << "1. Beri rating" << endl;
                    cout << "2. Hapus rating" << endl;
                    cout << "3. Kembali" << endl;
                }
                if (menu_review == 1) {
                    clearScreen();
                    if (jumlah_review >= maks_review) {
                        cout << MERAH << "Kapasitas review penuh!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    bool has_approved_room = false;
                    for (int i = 0; i < jumlah_kamar; i++) {
                        if (data_kamar[i].penghuni == data_user[id_user].nama && 
                            data_kamar[i].status == "terisi") {
                            has_approved_room = true;
                            break;
                        }
                    }
                    if (!has_approved_room) {
                        cout << MERAH << "Anda belum memiliki kamar!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    bool sudah_review = false;
                    for (int i = 0; i < jumlah_review; i++) {
                        if (data_review[i].nama_user == data_user[id_user].nama) {
                            sudah_review = true;
                            break;
                        }
                    }
                    if (sudah_review) {
                        cout << MERAH << "Anda sudah memberikan review!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    int rating;
                    while (!inputNumerik(rating, "Masukkan rating (1-5): ", 1, 5)) {
                        clearScreen();
                    }
                    cout << KUNING << "Masukkan komentar: " << RESET;
                    string komentar;
                    getline(cin, komentar);
                    if (komentar.length() > 100) {
                        cout << MERAH << "Komentar terlalu panjang (maks 100 karakter)!" << RESET << endl;
                        system("pause");
                        continue;
                    }
                    data_review[jumlah_review] = {data_user[id_user].nama, rating, komentar};
                    jumlah_review++;
                    cout << HIJAU << "Review berhasil ditambahkan!" << RESET << endl;
                    system("pause");
                } else if (menu_review == 2) {
                    clearScreen();
                    bool has_review = false;
                    for (int i = 0; i < jumlah_review; i++) {
                        if (data_review[i].nama_user == data_user[id_user].nama) {
                            has_review = true;
                            for (int j = i; j < jumlah_review - 1; j++) {
                                data_review[j] = data_review[j + 1];
                            }
                            jumlah_review--;
                            cout << HIJAU << "Review berhasil dihapus!" << RESET << endl;
                            system("pause");
                            break;
                        }
                    }
                    if (!has_review) {
                        cout << MERAH << "Anda belum memiliki review!" << RESET << endl;
                        system("pause");
                    }
                }
            } while (menu_review != 3);
        }
    } while (pilihan != 4);
}

int main() {
    data_user[0] = {"admin", "admin", "admin"};
    int pilihan;
    int autentikasi;

    do {
        clearScreen();
        cout << CYAN << "\n=== SISTEM MANAJEMEN KOST ===" << RESET << endl;
        cout << "1. Registrasi" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        while (!inputNumerik(pilihan, "Pilih: ", 1, 3)) {
            clearScreen();
            cout << CYAN << "\n=== SISTEM MANAJEMEN KOST ===" << RESET << endl;
            cout << "1. Registrasi" << endl;
            cout << "2. Login" << endl;
            cout << "3. Keluar" << endl;
        }
        if (pilihan == 1) {
            registrasi();
        } else if (pilihan == 2) {
            if (login(autentikasi)) {
                clearScreen();
                if (data_user[autentikasi].role == "admin") {
                    menu_admin();
                } else {
                    menu_user(autentikasi);
                }
            }
        } else if (pilihan == 3) {
            clearScreen();
            cout << KUNING << "Terima kasih telah menggunakan program!" << RESET << endl;
            system("pause");
        }
    } while (pilihan != 3);

    return 0;
}