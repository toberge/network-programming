package toberge.insecureBank;

import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

public class RaceCondition {
    private static Thread A, B;
    private static void sleep(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public static void main(String[] args) {
        EntityManagerFactory emf = null;
        try {
            emf = Persistence.createEntityManagerFactory("WormholePU");
            AccountFacadeImpl dao = new AccountFacadeImpl(new AccountDAO(emf));
            String NUMBER_ONE = "2323.23.23232";
            String NUMBER_TWO = "9999.11.88888";
            dao.addOne(new Account(NUMBER_ONE, 4000, "Gulbrand Gråstein"));
            dao.addOne(new Account(NUMBER_TWO, 2000, "Skrue McDuck"));
            A = new Thread(() -> {
                Account one = dao.getOne(NUMBER_ONE);
                Account two = dao.getOne(NUMBER_TWO);
                one.withdraw(1000);
                two.deposit(1000);
                sleep(1000);
                dao.updateOne(one);
                dao.updateOne(two);
            });

            B = new Thread(() -> {
                Account one = dao.getOne(NUMBER_ONE);
                Account two = dao.getOne(NUMBER_TWO);
                sleep(2000);
                one.withdraw(1000);
                two.deposit(1000);
                dao.updateOne(one);
                dao.updateOne(two);
            });
            A.start();
            B.start();
            sleep(3000);
            System.out.println(dao.getOne(NUMBER_ONE).getBalance() + " vs " + dao.getOne(NUMBER_TWO).getBalance());
        } finally {
            emf.close();
        }
    }
}
