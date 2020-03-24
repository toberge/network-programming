package toberge.lockedBank;

import javax.persistence.EntityManagerFactory;
import javax.persistence.OptimisticLockException;
import javax.persistence.Persistence;

public class OptimisticLockingTest {
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
            LockedAccountFacadeImpl dao = new LockedAccountFacadeImpl(new LockedAccountDAO(emf));
            String NUMBER_ONE = "2323.23.23232";
            String NUMBER_TWO = "9999.11.88888";
            dao.addOne(new LockedAccount(NUMBER_ONE, 4000, "Gulbrand Gråstein"));
            dao.addOne(new LockedAccount(NUMBER_TWO, 2000, "Skrue McDuck"));
            System.out.println(dao.getOne(NUMBER_ONE).getBalance() + " vs " + dao.getOne(NUMBER_TWO).getBalance());
            System.out.println("Transferring 1000$ from one to the other");
            A = new Thread(() -> {
                LockedAccount one = dao.getOne(NUMBER_ONE);
                LockedAccount two = dao.getOne(NUMBER_TWO);
                one.withdraw(1000);
                two.deposit(1000);
                sleep(1000);
                dao.updateOne(one);
                dao.updateOne(two);
            });

            B = new Thread(() -> {
                LockedAccount one = dao.getOne(NUMBER_ONE);
                LockedAccount two = dao.getOne(NUMBER_TWO);
                sleep(2000);
                one.withdraw(1000);
                two.deposit(1000);
                try {
                    // this will fail
                    dao.updateOne(one);
                    dao.updateOne(two);
                } catch (OptimisticLockException e) {
                    System.err.println(e);
                    // repeating the entire procedure
                    one = dao.getOne(NUMBER_ONE);
                    two = dao.getOne(NUMBER_TWO);
                    one.withdraw(1000);
                    two.deposit(1000);
                    dao.updateOne(one);
                    dao.updateOne(two);
                }
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
