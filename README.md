# 🎵 MP3 Tag Reader & Editor (C Project)

## 📌 Overview

MP3 Tag Reader & Editor is a command-line application developed in C that allows users to read and modify ID3v2 metadata of MP3 files.

The project uses binary file handling to safely process MP3 frames and includes a backup mechanism to preserve the original file before editing.

---

## ✨ Features

- View MP3 metadata:
  - Title
  - Artist
  - Album
  - Year
  - Comment
- Edit selected ID3 tags
- Automatic backup creation before editing
- Safe file handling using temporary file method
- Command-line based interface
- Error handling for invalid inputs

---

## 🛠 Technologies Used

- C Programming
- Binary File Handling
- ID3v2 Tag Structure
- Command Line Arguments
- Modular Programming

---

## 🚀 How to Compile

gcc *.c

---

## ▶️ How to Run

### 🔹 View MP3 Tags

./a.out -v sample.mp3

### 🔹 Edit MP3 Tags

./a.out -e -t "New Title" sampal.mp3

Available edit options:

- -t → Title  
- -a → Artist  
- -A → Album  
- -y → Year  
- -c → Comment  

### 🔹 Help

./a.out --help

---

## 🔒 Backup Mechanism

Before editing any MP3 file:

- A backup copy is created  
- The original file is preserved  
- Editing is done safely using a temporary file  

Example:

song.mp3          → Edited file  
backup_song.mp3   → Original backup  

---

## 📂 Project Structure

.
├── main.c  
├── view.c  
├── edit.c  
├── main.h  
└── README.md  

---
