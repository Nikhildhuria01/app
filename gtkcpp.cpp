#include <gtk/gtk.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// ---- Student Class ----
class Student {
    string name, roll_no;
    int marks[10];
public:
    Student() : name(""), roll_no("") { fill(marks, marks+10, 0); }
    Student(const string& n, const string& r, const int m[10]) : name(n), roll_no(r) { copy(m, m+10, marks); }
    string getName() const { return name; }
    string getRollNo() const { return roll_no; }
    int getMark(int i) const { return marks[i]; }
    void setName(const string& n) { name = n; }
    void setMarks(const int m[10]) { copy(m, m+10, marks); }
    float avg() const { float s=0; for(int i=0;i<10;++i) s+=marks[i]; return s/10.0f; }
    string grade(int m) const {
        if(m>=90) return "A";
        if(m>=80) return "B";
        if(m>=70) return "C";
        if(m>=60) return "D";
        if(m>=50) return "E";
        return "F";
    }
    int backlogs() const {
        int b=0; for(int i=0;i<10;++i) if(grade(marks[i])=="F") ++b; return b;
    }
    string suggest() const {
        float a=avg();
        if(a>=85 && marks[1]>=90 && marks[6]>=85) return "AIML/FULL STACK";
        if(a>=80 && marks[2]>=85 && marks[3]>=85 && marks[6]>=80) return "DATA SCIENCE";
        if(a>=75 && marks[4]>=80 && marks[8]>=75 && marks[9]>=75) return "IOT/ELECTRONICS";
        return "CYBERSECURITY";
    }
    void save(ofstream& out) const {
        out << name << '\n' << roll_no << '\n';
        for(int i=0;i<10;++i) out << marks[i] << (i<9?' ':'\n');
    }
    bool load(ifstream& in) {
        if(!getline(in, name)) return false;
        if(!getline(in, roll_no)) return false;
        for(int i=0;i<10;++i) in >> marks[i];
        in.ignore(1000, '\n');
        return true;
    }
};

// ---- Globals ----
vector<Student> students;
const string datafile = "student_data.txt";
GtkWidget *main_window, *text_view;
GtkTextBuffer *text_buffer;

const char* subj[10] = {"C", "C++", "m1", "m2", "phy", "chem", "ps", "Comm", "BEEE", "BME"};
const char* subj_full[10] = {"C", "C++", "Maths1", "Maths2", "Physics", "Chemistry", "ProbSolv", "Comm", "BEEE", "BME"};
const char* subj_color[10] = {"#1abc9c", "#3498db", "#e67e22", "#9b59b6", "#e74c3c", "#16a085", "#f39c12", "#2ecc71", "#34495e", "#8e44ad"};

// ---- File Operations ----
void save_students() {
    ofstream out(datafile);
    for(const auto& s: students) s.save(out);
}
void load_students() {
    students.clear();
    ifstream in(datafile);
    Student s;
    while(s.load(in)) students.push_back(s);
}

// ---- Utility ----
void show_markup(const string& markup) {
    gtk_text_buffer_set_text(text_buffer, "", -1);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    gtk_text_buffer_insert_markup(text_buffer, &end, markup.c_str(), -1);
}
bool roll_exists(const string& roll) {
    for(const auto& s: students) if(s.getRollNo()==roll) return true;
    return false;
}
bool valid_marks(const int m[10]) {
    for(int i=0;i<10;++i) if(m[i]<0||m[i]>100) return false;
    return true;
}

// ---- Login Dialog ----
bool login_dialog() {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Login", NULL, GTK_DIALOG_MODAL, "Login", GTK_RESPONSE_OK, "Exit", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(content), grid);

    GtkWidget *icon = gtk_image_new_from_icon_name("dialog-password", GTK_ICON_SIZE_DIALOG);
    gtk_grid_attach(GTK_GRID(grid), icon, 0, 0, 2, 1);

    GtkWidget *user_entry = gtk_entry_new();
    GtkWidget *pass_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Username:"), 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), user_entry, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password:"), 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid), pass_entry, 1,2,1,1);

    GtkWidget *anim = gtk_spinner_new();
    gtk_grid_attach(GTK_GRID(grid), anim, 0,3,2,1);

    gtk_widget_show_all(dialog);
    bool success = false;
    while(true) {
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if(res != GTK_RESPONSE_OK) break;
        gtk_spinner_start(GTK_SPINNER(anim));
        string user = gtk_entry_get_text(GTK_ENTRY(user_entry));
        string pass = gtk_entry_get_text(GTK_ENTRY(pass_entry));
        g_usleep(400000); // Animation delay
        gtk_spinner_stop(GTK_SPINNER(anim));
        if(user=="admin" && pass=="admin123") {
            gdk_beep(); // Play success sound
            success=true;
            break;
        }
        gdk_beep(); // Play sound for error
        GtkWidget *err = gtk_message_dialog_new(GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid credentials!");
        gtk_dialog_run(GTK_DIALOG(err));
        gtk_widget_destroy(err);
    }
    gtk_widget_destroy(dialog);
    return success;
}

// ---- Add/Edit/Delete Student ----
void add_student(GtkWidget*, gpointer);
void edit_student(GtkWidget*, gpointer);
void delete_student(GtkWidget*, gpointer);

// ---- Display Functions ----
void display_students(GtkWidget*, gpointer);
void display_students_ascii(GtkWidget*, gpointer);
void display_avg_ascii(GtkWidget*, gpointer);
void display_bar_graph(GtkWidget*, gpointer);
void suggest_specialization(GtkWidget*, gpointer);
void mentorship_recommendation(GtkWidget*, gpointer);
void deans_list(GtkWidget*, gpointer);
void clear_display(GtkWidget*, gpointer) { gtk_text_buffer_set_text(text_buffer, "", -1); }
void exit_app(GtkWidget*, gpointer) { gtk_main_quit(); }

// ---- Leaderboard Function ----
void leaderboard(GtkWidget*, gpointer) {
    if(students.empty()) {
        show_markup("<span foreground='#e74c3c'><b>No students available!</b></span>");
        return;
    }
    vector<pair<float,int>> avgs;
    for(size_t i=0;i<students.size();++i)
        avgs.push_back({students[i].avg(), (int)i});
    sort(avgs.rbegin(), avgs.rend()); // Descending

    ostringstream oss;
    oss << "<tt><b>Leaderboard (All Students)</b>\n";
    oss << left << setw(3) << "" << setw(15) << "Name" << setw(10) << "RollNo" << setw(8) << "Avg" << "\n";
    oss << string(3+15+10+8, '-') << "\n";
    for(size_t i=0;i<avgs.size();++i) {
        string star;
        string color;
        if(i==0) { star="★"; color="#FFD700"; } // Gold
        else if(i==1) { star="★"; color="#C0C0C0"; } // Silver
        else if(i==2) { star="★"; color="#CD7F32"; } // Bronze
        else { star=""; color="#000000"; }
        int idx = avgs[i].second;
        oss << "<span foreground='" << color << "'>" << left << setw(3) << star << "</span>"
            << left << setw(15) << students[idx].getName()
            << left << setw(10) << students[idx].getRollNo()
            << left << setw(8) << fixed << setprecision(1) << students[idx].avg() << "\n";
    }
    oss << "</tt>";
    show_markup(oss.str());
}

// ---- Add/Edit/Delete Student (implementations) ----
void add_student(GtkWidget*, gpointer) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Student", GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL, "_Add", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5); gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);

    GtkWidget *name_entry = gtk_entry_new(), *roll_entry = gtk_entry_new();
    GtkWidget *entries[10];
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Roll No:"), 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), roll_entry, 1,1,1,1);
    for(int i=0;i<10;++i) {
        entries[i]=gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new(subj_full[i]), 0,i+2,1,1);
        gtk_grid_attach(GTK_GRID(grid), entries[i], 1,i+2,1,1);
    }
    gtk_widget_show_all(dialog);
    if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK) {
        string name=gtk_entry_get_text(GTK_ENTRY(name_entry));
        string roll=gtk_entry_get_text(GTK_ENTRY(roll_entry));
        int m[10];
        for(int i=0;i<10;++i) m[i]=atoi(gtk_entry_get_text(GTK_ENTRY(entries[i])));
        if(name.empty()||roll.empty()) {
            gdk_beep(); // Play sound for error
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Fields cannot be empty!");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
        }
        else if(roll_exists(roll)) {
            gdk_beep(); // Play sound for error
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Roll number already exists!");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
        }
        else if(!valid_marks(m)) {
            gdk_beep(); // Play sound for error
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid marks! Enter 0-100.");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
        }
        else {
            students.emplace_back(name, roll, m);
            save_students();
            gdk_beep(); // Play sound for success
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Student added successfully!");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
            show_markup("<span foreground='#27ae60'><b>Student added successfully!</b></span>");
        }
    }
    gtk_widget_destroy(dialog);
}
void edit_student(GtkWidget*, gpointer) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Edit Student", GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL, "_Search", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *roll_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), roll_entry);
    gtk_widget_show_all(dialog);
    if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK) {
        string roll=gtk_entry_get_text(GTK_ENTRY(roll_entry));
        auto it=find_if(students.begin(),students.end(),[&](const Student& s){return s.getRollNo()==roll;});
        if(it==students.end()) {
            gdk_beep(); // Play sound for error
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student not found!");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
        }
        else {
            gtk_widget_destroy(dialog);
            GtkWidget *ed=gtk_dialog_new_with_buttons("Edit Details", GTK_WINDOW(main_window),
                GTK_DIALOG_MODAL, "_Update", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
            GtkWidget *grid=gtk_grid_new();
            gtk_grid_set_row_spacing(GTK_GRID(grid), 5); gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
            gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(ed))), grid);
            GtkWidget *name_entry=gtk_entry_new(), *entries[10];
            gtk_entry_set_text(GTK_ENTRY(name_entry), it->getName().c_str());
            gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0,0,1,1);
            gtk_grid_attach(GTK_GRID(grid), name_entry, 1,0,1,1);
            for(int i=0;i<10;++i) {
                entries[i]=gtk_entry_new();
                gtk_entry_set_text(GTK_ENTRY(entries[i]), to_string(it->getMark(i)).c_str());
                gtk_grid_attach(GTK_GRID(grid), gtk_label_new(subj_full[i]), 0,i+1,1,1);
                gtk_grid_attach(GTK_GRID(grid), entries[i], 1,i+1,1,1);
            }
            gtk_widget_show_all(ed);
            if(gtk_dialog_run(GTK_DIALOG(ed))==GTK_RESPONSE_OK) {
                string name=gtk_entry_get_text(GTK_ENTRY(name_entry));
                int m[10]; for(int i=0;i<10;++i) m[i]=atoi(gtk_entry_get_text(GTK_ENTRY(entries[i])));
                if(name.empty()) {
                    gdk_beep(); // Play sound for error
                    GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name cannot be empty!");
                    gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
                }
                else if(!valid_marks(m)) {
                    gdk_beep(); // Play sound for error
                    GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid marks! Enter 0-100.");
                    gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
                }
                else {
                    it->setName(name); it->setMarks(m); save_students();
                    gdk_beep(); // Play sound for success
                    show_markup("<span foreground='#27ae60'><b>Student updated successfully!</b></span>");
                }
            }
            gtk_widget_destroy(ed);
            return;
        }
    }
    gtk_widget_destroy(dialog);
}
void delete_student(GtkWidget*, gpointer) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete Student", GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL, "_Delete", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *roll_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), roll_entry);
    gtk_widget_show_all(dialog);
    if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK) {
        string roll=gtk_entry_get_text(GTK_ENTRY(roll_entry));
        auto it=remove_if(students.begin(),students.end(),[&](const Student& s){return s.getRollNo()==roll;});
        if(it!=students.end()) {
            students.erase(it,students.end());
            save_students();
            gdk_beep(); // Play sound for success
            show_markup("<span foreground='#e74c3c'><b>Student deleted.</b></span>");
        } else {
            gdk_beep(); // Play sound for error
            GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student not found!");
            gtk_dialog_run(GTK_DIALOG(msg)); gtk_widget_destroy(msg);
        }
    }
    gtk_widget_destroy(dialog);
}

// ---- Display Functions ----
void display_students(GtkWidget*, gpointer) {
    ostringstream oss;
    oss << "<tt>";
    oss << left << setw(15) << "Name" << setw(10) << "RollNo";
    for(int i=0;i<10;++i)
        oss << "<span foreground='" << subj_color[i] << "'>" << setw(7) << subj[i] << "</span>";
    oss << setw(8) << "Avg" << "\n";
    oss << string(15+10+7*10+8,'-') << "\n";
    for(const auto& s: students) {
        oss << setw(15) << s.getName() << setw(10) << s.getRollNo();
        for(int i=0;i<10;++i) oss << setw(7) << s.getMark(i);
        oss << setw(8) << fixed << setprecision(1) << s.avg() << "\n";
    }
    oss << "</tt>";
    show_markup(oss.str());
}

void display_students_ascii(GtkWidget*, gpointer) {
    ostringstream oss;
    oss << "<tt><b>ASCII Table</b>\n";
    oss << left << setw(15) << "Name" << setw(10) << "RollNo";
    for(int i=0;i<10;++i) oss << left << setw(7) << subj[i];
    oss << left << setw(8) << "Avg" << left << setw(20) << "Avg ASCII" << "\n";
    oss << string(15+10+7*10+8+20, '-') << "\n";
    for(const auto& s: students) {
        oss << left << setw(15) << s.getName() << left << setw(10) << s.getRollNo();
        for(int i=0;i<10;++i) oss << left << setw(7) << s.getMark(i);
        float avg = s.avg();
        oss << left << setw(8) << fixed << setprecision(1) << avg;
        // Convert average to string and get ASCII values
        ostringstream avg_ss;
        avg_ss << fixed << setprecision(1) << avg;
        string avg_str = avg_ss.str();
        ostringstream ascii_ss;
        for(char c : avg_str) {
            ascii_ss << (int)c << " ";
        }
        oss << left << setw(20) << ascii_ss.str() << "\n";
    }
    oss << "</tt>";
    show_markup(oss.str());
}

void display_avg_ascii(GtkWidget*, gpointer) {
    ostringstream oss;
    oss << "<tt><b>Average Table</b>\n";
    oss << left << setw(10) << "RollNo" << setw(15) << "Name" << setw(8) << "Average" << "\n";
    oss << string(10+15+8, '-') << "\n";
    for(const auto& s: students) {
        oss << left << setw(10) << s.getRollNo() << setw(15) << s.getName()
            << setw(8) << fixed << setprecision(1) << s.avg() << "\n";
    }
    oss << "</tt>";
    show_markup(oss.str());
}

void display_bar_graph(GtkWidget*, gpointer) {
    if(students.empty()) {
        show_markup("<span foreground='#e74c3c'><b>No students available!</b></span>");
        return;
    }
    ostringstream oss;
    oss << "Bar Graph of Averages\n";
    oss << string(40,'-') << "\n";
    for(const auto& s: students) {
        int bars = (int)(s.avg()/2); // 50 bars max for 100 marks
        oss << setw(15) << s.getName() << " | ";
        for(int i=0;i<bars;++i) oss << "█";
        oss << " " << fixed << setprecision(1) << s.avg() << "\n";
    }
    show_markup("<tt>" + oss.str() + "</tt>");
}

void suggest_specialization(GtkWidget*, gpointer) {
    if(students.empty()) {
        show_markup("<span foreground='#e74c3c'><b>No students available!</b></span>");
        return;
    }
    ostringstream oss;
    oss << "<tt><b>Specialization Suggestions</b>\n";
    oss << left << setw(15) << "Name" << setw(10) << "RollNo";
    for(int i=0;i<10;++i)
        oss << "<span foreground='" << subj_color[i] << "'>" << setw(7) << subj[i] << "</span>";
    oss << setw(8) << "Avg" << setw(20) << "Specialization" << "\n";
    oss << string(15+10+7*10+8+20,'-') << "\n";
    for(const auto& s: students) {
        oss << setw(15) << s.getName() << setw(10) << s.getRollNo();
        for(int i=0;i<10;++i) oss << setw(7) << s.getMark(i);
        oss << setw(8) << fixed << setprecision(1) << s.avg()
            << "<span foreground='#2980b9'>" << setw(20) << s.suggest() << "</span>\n";
    }
    oss << "</tt>";
    show_markup(oss.str());
}

void mentorship_recommendation(GtkWidget*, gpointer) {
    if(students.empty()) {
        show_markup("<span foreground='#e74c3c'><b>No students available!</b></span>");
        return;
    }
    ostringstream oss;
    oss << "<tt><b>Mentorship Recommendation</b>\n";
    oss << left << setw(15) << "Name" << setw(10) << "RollNo" << setw(8) << "Avg" << setw(10) << "Backlogs\n";
    oss << string(15+10+8+10,'-') << "\n";
    for(const auto& s: students) {
        if(s.avg() < 50) {
            oss << "<span foreground='#e67e22'>" 
                << setw(15) << s.getName() 
                << setw(10) << s.getRollNo()
                << setw(8) << fixed << setprecision(1) << s.avg()
                << setw(10) << s.backlogs() << "</span>\n";
        }
    }
    oss << "</tt>";
    show_markup(oss.str());
}

void deans_list(GtkWidget*, gpointer) {
    if(students.empty()) {
        show_markup("<span foreground='#e74c3c'><b>No students available!</b></span>");
        return;
    }
    ostringstream oss;
    oss << "<tt><b>Dean's List (Avg > 85, No backlogs)</b>\n";
    oss << left << setw(15) << "Name" << setw(10) << "RollNo" << setw(8) << "Avg\n";
    oss << string(15+10+8,'-') << "\n";
    for(const auto& s: students) {
        if(s.avg() > 85 && s.backlogs() == 0) {
            oss << "<span foreground='#27ae60'>" 
                << setw(15) << s.getName() 
                << setw(10) << s.getRollNo()
                << setw(8) << fixed << setprecision(1) << s.avg() << "</span>\n";
        }
    }
    oss << "</tt>";
    show_markup(oss.str());
}

// ---- Main Window ----
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    load_students();

    if(!login_dialog()) return 0;

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Student Management System");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1200, 700);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 15);
    g_signal_connect(main_window, "destroy", G_CALLBACK(exit_app), NULL);

    // Apply a simple CSS style
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #00008B; }"
        "button { padding: 10px; margin: 5px; font-size: 14px; border-radius: 5px; background-color: #FFFF00; color: black; transition: transform 0.2s ease, background-color 0.2s ease; }"
        "button:hover { background-color: #E6E600; transform: scale(1.1); }"
        "textview { font-family: monospace; font-size: 12px; padding: 10px; background-color: #E6E6FA; border: 1px solid #ddd; }"
        "scroll { border-radius: 5px; }", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Outer horizontal box: left = buttons, right = text view
    GtkWidget *hbox_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), hbox_main);

    // Vertical button box (left)
    GtkWidget *vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_buttons, FALSE, FALSE, 0);

    struct { const char* label; void(*cb)(GtkWidget*,gpointer); } ops[] = {
        {"Add", add_student}, {"Edit", edit_student}, {"Delete", delete_student},
        {"Display", display_students}, {"ASCII Table", display_students_ascii},
        {"Avg Table", display_avg_ascii}, {"Bar Graph", display_bar_graph},
        {"Leaderboard", leaderboard}, {"Suggest", suggest_specialization},
        {"Mentorship", mentorship_recommendation}, {"Dean's List", deans_list},
        {"Clear", clear_display}, {"Exit", exit_app}
    };
    for(auto& op: ops) {
        GtkWidget *btn = gtk_button_new_with_label(op.label);
        g_signal_connect(btn, "clicked", G_CALLBACK(op.cb), NULL);
        gtk_box_pack_start(GTK_BOX(vbox_buttons), btn, FALSE, FALSE, 0);
    }

    // Text view (right)
    GtkWidget *vbox_right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_right, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(vbox_right), scroll, TRUE, TRUE, 0);

    gtk_widget_show_all(main_window);
    gtk_main();
    return 0;}
