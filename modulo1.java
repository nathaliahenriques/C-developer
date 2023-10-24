import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;

class ContaBancaria {
    private String tipo;
    private double saldo;

    public ContaBancaria(String tipo, double saldoInicial) {
        this.tipo = tipo;
        this.saldo = saldoInicial;
    }

    public void sacar(double valor) {
        if (valor > saldo) {
            throw new SaldoInsuficienteException("Saldo insuficiente na conta " + tipo);
        } else {
            saldo -= valor;
            System.out.println("Saque de " + valor + " realizado na conta " + tipo);
        }
        
    }

    public double getSaldo() {
        return saldo;
    }
}

class SaldoInsuficienteException extends RuntimeException {
    public SaldoInsuficienteException(String message) {
        super(message);
    }
}

class ContaCorrente extends ContaBancaria {
    public ContaCorrente(double saldoInicial) {
        super("Corrente", saldoInicial);
    }
}

class ContaSalario extends ContaBancaria {
    public ContaSalario(double saldoInicial) {
        super("Salário", saldoInicial);
    }
}

class ContaPoupanca extends ContaBancaria {
    public ContaPoupanca(double saldoInicial) {
        super("Poupança", saldoInicial);
    }
}

@Aspect
class VerificarSaldoAspect {
    @Before("call(public void ContaBancaria.sacar(double))")
    public void verificarSaldo() {
        // Adicione aqui a lógica para verificar o saldo e, se necessário, lançar uma exceção
    }
}

public class ProgramaPrincipal {
    public static void main(String[] args) {
        // Crie instâncias das diferentes contas bancárias
        ContaCorrente contaCorrente = new ContaCorrente(1000.0);
        ContaSalario contaSalario = new ContaSalario(500.0);
        ContaPoupanca contaPoupanca = new ContaPoupanca(1500.0);

        // Realize operações de saque
        try {
            contaCorrente.sacar(800.0); // Deve permitir o saque
            contaSalario.sacar(600.0);  // Deve gerar uma exceção SaldoInsuficienteException
            contaPoupanca.sacar(2000.0); // Deve gerar uma exceção SaldoInsuficienteException
        } catch (SaldoInsuficienteException e) {
            System.out.println("Erro: " + e.getMessage());
        }
    }
}
