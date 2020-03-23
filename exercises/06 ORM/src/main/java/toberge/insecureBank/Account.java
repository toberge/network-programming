package toberge.insecureBank;
import javax.persistence.*;
import java.io.*;
import java.util.regex.*;

@Entity @NamedQuery(name="findNumberOfAccounts", query = "SELECT COUNT(a) FROM Account a")

public class Account implements Serializable {
    @Id
    private String accountNumber; // String for letthets skyld
    private double balance;
    private String owner;

    // See https://no.wikipedia.org/wiki/Kontonummer
    private static final Pattern ACCOUNT_NUMBER_PATTERN = Pattern.compile("\\d{4}\\.\\d{2}\\.\\d{5}");

    public Account() {}
    public Account(String accountNumber, double balance, String owner) {
        if (!ACCOUNT_NUMBER_PATTERN.matcher(accountNumber).matches()) {
            throw new IllegalArgumentException("Account number should be of the form:\n'xxxx.yy.zzzzc'");
        }
        this.accountNumber = accountNumber;
        this.balance = balance;
        this.owner = owner;
    }

    public String getAccountNumber() {
        return accountNumber;
    }

    public void setAccountNumber(String accountNumber) {
        this.accountNumber = accountNumber;
    }

    public double getBalance() {
        return balance;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public void withdraw(double amount) {
        this.balance -= amount;
    }

    public void deposit(double amount) {
        this.balance += amount;
    }

    @Override
    public String toString() {
        return "Account{" +
                "accountNumber='" + accountNumber + '\'' +
                ", balance=" + balance +
                ", owner='" + owner + '\'' +
                '}';
    }
}
