package toberge.insecureBank;
import javax.persistence.*;
import java.util.List;

public class AccountDAO {
    private EntityManagerFactory emf; // avoiding thread problems...

    public AccountDAO(EntityManagerFactory emf) {
        this.emf = emf;
    }

    private EntityManager getEM() {
        return emf.createEntityManager();
    }

    private void closeEM(EntityManager em) {
        if (em != null && em.isOpen()) em.close();
    }


    public Account getOne(String accountNumber) {
        EntityManager em = getEM();
        try {
            return em.find(Account.class, accountNumber);
        } finally {
            closeEM(em);
        }
    }

    public void addOne(Account account) {
        EntityManager em = getEM();
        try {
            em.getTransaction().begin();
            em.persist(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public void updateOne(Account account) {
        EntityManager em = getEM();
        try {
            em.getTransaction().begin();
            em.merge(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public void deleteOne(String accountNumber) {
        EntityManager em = getEM();
        try {
            Account account = em.find(Account.class, accountNumber);
            em.getTransaction().begin();
            em.remove(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public List<Account> getAll() {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM Account a");
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }

    public List<Account> getAllLargerThan(double amount) {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM Account a WHERE a.balance > :amount");
            query.setParameter("amount", amount);
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }


    public List<Account> getAllByOwner(String owner) {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM Account a WHERE a.owner = :owner");
            query.setParameter("owner", owner);
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }

    public int getNumberOfAccounts() {
        EntityManager em = getEM();
        try{
            Query q = em.createNamedQuery("findNumberOfAccounts");
            Long count = (Long) q.getSingleResult();
            return count.intValue();
        }finally{
            closeEM(em);
        }
    }

    public static void main(String[] args) {
        EntityManagerFactory emf = null;
        try {
            emf = Persistence.createEntityManagerFactory("WormholePU");
            AccountDAO dao = new AccountDAO(emf);
            dao.addOne(new Account("1234.23.12345", 34000.0, "Olram Slåpen"));
            dao.addOne(new Account("6666.44.66667", 25000.0, "Emanuel Desperados"));
            List<Account> them = dao.getAll();
            for (Account a : them) {
                System.out.println(a);
            }
        } finally {
            if (emf != null) emf.close();
        }
    }
}
