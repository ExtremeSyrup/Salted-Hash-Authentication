# Salted Hash Authentication Mechanism

## Overview

This project implements a salted hash authentication mechanism to securely store and validate user passwords. The use of salts adds an extra layer of security by preventing rainbow table attacks and making it more challenging for attackers to guess passwords.

## How it Works

1. **Password Hashing:** When a user creates an account or updates their password, the system generates a unique salt for that user. The password is then combined with the salt and hashed using a cryptographic hash function (e.g., SHA-256).

2. **Storage:** The system stores the salt and the hashed password in the database. The salt is stored alongside the hash to ensure that each password has a unique hash, even if two users have the same password.

3. **Authentication:** When a user logs in, the system retrieves the salt associated with the user's account from the database. It then combines the entered password with the retrieved salt and hashes it using the same hash function. If the resulting hash matches the stored hash, the password is considered valid.

## Benefits

- **Security:** Salting passwords significantly improves security by making it harder for attackers to crack passwords, even if they have access to the hashed passwords.
  
- **Individualized Hashes:** Each user has a unique salt, ensuring that even users with the same password will have different hash values stored in the database.

- **Resistance to Rainbow Table Attacks:** Rainbow table attacks, which involve precomputed hashes of commonly used passwords, are mitigated because the salt adds a unique element to each password before hashing.

## Prerequisites

- [MySQL](https://www.mysql.com/) or any other compatible database system installed on your machine.

## Getting Started
1. Clone the Salted-Hash-Authentication repository:
2. ```git clone https://github.com/ExtremeSyrup/Salted-Hash-Authentication```
3. you must create a Database, and create a table with name 'users' as described below
```
CREATE TABLE your_database_name.users (
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(128) NOT NULL,
    salt VARCHAR(32) NOT NULL,
    PRIMARY KEY (username)
);
```
4. Include the user.h header file from the cloned repository in your project to enable salted hash authentication.
5. Follow the steps according to main.cpp for interfacing the headerfile.
