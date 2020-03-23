package toberge.insecureBank;

import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.List;

public class ImplementationTest {
    public static void main(String[] args) {
        EntityManagerFactory emf = null;
        try {
            emf = Persistence.createEntityManagerFactory("WormholePU");
            AccountFacadeImpl dao = new AccountFacadeImpl(new AccountDAO(emf));

            // Tid for å gi Bronskimlet sin velfortjente diktergasje
            double[] balances = new double[]{200, 300, 400, 500, 600, 700, 800, 900, 1000};
            for (double balance : balances) {
                dao.addOne(new Account(String.format("%04d.23.%04d5", (int) balance, (int) balance), balance, "Hallstein Bronskimlet"));
            }
            List<Account> them = dao.getAllLargerThan(800);
            for (Account a : them) {
                System.out.println(a);
            }
            // Rename one then verify
            Account renamed = them.get(1);
            renamed.setOwner("Olram Slåpen");
            dao.updateOne(renamed);
            them = dao.getAllByOwner("Olram Slåpen");
            assert them.size() == 1;
            assert them.get(0).getOwner().equals("Olram Slåpen");
        } finally {
            if (emf != null) emf.close();
        }
    }
}
