package com.zeldaonline.host;

import android.Manifest;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;

public final class MainActivity extends Activity {
    private final Handler handler = new Handler(Looper.getMainLooper());
    private EditText portField, daySpeedField, nightSpeedField, resetTimerField;
    private CheckBox dungeonResetCheck;
    private TextView statusText, lanAddressText;
    private Button startButton, stopButton;
    private int lastStatus = -1;

    private final Runnable refresh = new Runnable() {
        @Override public void run() {
            int status = ServerNative.status();
            if (status != lastStatus) {
                lastStatus = status;
                renderStatus(status);
            }
            handler.postDelayed(this, 700);
        }
    };

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(Color.rgb(12, 16, 24));
        getWindow().setNavigationBarColor(Color.rgb(12, 16, 24));
        buildUi();
        getSharedPreferences("host", MODE_PRIVATE).edit()
                .putInt("port", readInt(portField, 21050)).apply();
        requestNotificationPermissionIfNeeded();
    }

    @Override protected void onResume() {
        super.onResume();
        lanAddressText.setText("IP desta rede: " + getLocalIp() + "\nMesmo aparelho: 127.0.0.1");
        handler.removeCallbacks(refresh);
        handler.post(refresh);
    }

    @Override protected void onPause() {
        handler.removeCallbacks(refresh);
        super.onPause();
    }

    private void buildUi() {
        ScrollView scroll = new ScrollView(this);
        scroll.setFillViewport(true);
        LinearLayout page = new LinearLayout(this);
        page.setOrientation(LinearLayout.VERTICAL);
        page.setPadding(dp(20), dp(18), dp(20), dp(24));
        page.setBackgroundColor(Color.rgb(12, 16, 24));
        scroll.addView(page);
        setContentView(scroll);

        TextView title = text("ZELDA ONLINE", 26, true, Color.WHITE);
        page.addView(title);
        TextView subtitle = text("HOST NATIVO PARA ANDROID", 12, true, Color.rgb(91, 198, 176));
        subtitle.setPadding(0, dp(3), 0, dp(18));
        page.addView(subtitle);

        LinearLayout statusCard = card();
        statusText = text("Servidor parado", 16, true, Color.WHITE);
        statusCard.addView(statusText);
        TextView statusHint = text("O servidor continua ativo com uma notificação persistente. Use Parar para encerrar.", 12, false, Color.rgb(184, 193, 207));
        statusHint.setPadding(0, dp(6), 0, 0);
        statusCard.addView(statusHint);
        page.addView(statusCard, gapParams(0, 0, 0, 16));

        TextView networkHeader = section("REDE");
        page.addView(networkHeader);
        portField = field("Porta TCP", "21050", true);
        page.addView(portField, gapParams(0, 0, 0, 10));
        lanAddressText = text("Obtendo IP da rede...", 13, false, Color.rgb(184, 193, 207));
        LinearLayout addressCard = card();
        addressCard.addView(lanAddressText);
        Button copyButton = button("COPIAR ENDEREÇO LOCAL");
        copyButton.setOnClickListener(v -> {
            String address = getLocalIp() + ":" + readInt(portField, 21050);
            ClipboardManager clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
            if (clipboard != null) clipboard.setPrimaryClip(ClipData.newPlainText("Zelda Online Host", address));
            Toast.makeText(this, "Endereço copiado: " + address, Toast.LENGTH_SHORT).show();
        });
        addressCard.addView(copyButton, gapParams(0, 12, 0, 0));
        page.addView(addressCard, gapParams(0, 0, 0, 18));

        page.addView(section("OPÇÕES DO SERVIDOR"));
        TextView timeHint = text("Velocidade do relógio do mundo (configurações do servidor)", 12, false, Color.rgb(184, 193, 207));
        timeHint.setPadding(0, dp(5), 0, dp(8));
        page.addView(timeHint);
        LinearLayout speeds = new LinearLayout(this);
        speeds.setOrientation(LinearLayout.HORIZONTAL);
        daySpeedField = field("Velocidade de dia", "160", true);
        nightSpeedField = field("Velocidade de noite", "320", true);
        LinearLayout.LayoutParams half = new LinearLayout.LayoutParams(0, dp(54), 1f);
        half.setMargins(0, 0, dp(6), dp(10));
        speeds.addView(daySpeedField, half);
        LinearLayout.LayoutParams half2 = new LinearLayout.LayoutParams(0, dp(54), 1f);
        half2.setMargins(dp(6), 0, 0, dp(10));
        speeds.addView(nightSpeedField, half2);
        page.addView(speeds);

        dungeonResetCheck = new CheckBox(this);
        dungeonResetCheck.setText("Resetar masmorras quando a sala fica vazia");
        dungeonResetCheck.setTextColor(Color.WHITE);
        dungeonResetCheck.setButtonTintList(android.content.res.ColorStateList.valueOf(Color.rgb(91, 198, 176)));
        dungeonResetCheck.setChecked(true);
        page.addView(dungeonResetCheck, gapParams(0, 0, 0, 6));
        resetTimerField = field("Tempo até resetar (minutos)", "10", true);
        page.addView(resetTimerField, gapParams(0, 0, 0, 18));

        startButton = button("INICIAR HOST");
        startButton.setBackground(background(Color.rgb(30, 166, 132), 12));
        startButton.setOnClickListener(v -> startHost());
        page.addView(startButton, gapParams(0, 0, 0, 10));
        stopButton = button("PARAR SERVIDOR");
        stopButton.setBackground(background(Color.rgb(177, 62, 70), 12));
        stopButton.setOnClickListener(v -> stopHost());
        page.addView(stopButton, gapParams(0, 0, 0, 18));

        TextView instructions = text("COMO CONECTAR\n1. Inicie o host aqui.\n2. Abra Shipwright-Online no celular do jogador.\n3. Entre em Network → Zelda Online e conecte usando o IP e a porta acima.\n4. Para o próprio celular conectar ao host, use 127.0.0.1.\n\nPara amigos fora da sua rede Wi-Fi, o roteador precisa encaminhar a porta TCP escolhida ou todos devem usar uma VPN/túnel compatível. O IP local sozinho não é acessível pela internet.", 13, false, Color.rgb(184, 193, 207));
        instructions.setPadding(dp(14), dp(14), dp(14), dp(14));
        instructions.setBackground(background(Color.rgb(25, 31, 43), 12));
        page.addView(instructions);
    }

    private void startHost() {
        hideKeyboard();
        int port = readInt(portField, -1);
        int day = readInt(daySpeedField, -1);
        int night = readInt(nightSpeedField, -1);
        int timer = readInt(resetTimerField, -1);
        if (port < 1 || port > 65535) { portField.setError("Use uma porta de 1 a 65535"); return; }
        if (day < 1 || night < 1) { Toast.makeText(this, "Velocidades precisam ser maiores que zero", Toast.LENGTH_LONG).show(); return; }
        if (timer < 1) { resetTimerField.setError("Use pelo menos 1 minuto"); return; }

        getSharedPreferences("host", MODE_PRIVATE).edit().putInt("port", port).apply();
        Intent service = new Intent(this, HostService.class)
                .setAction(HostService.ACTION_START)
                .putExtra(HostService.EXTRA_PORT, port)
                .putExtra(HostService.EXTRA_DAY_SPEED, day)
                .putExtra(HostService.EXTRA_NIGHT_SPEED, night)
                .putExtra(HostService.EXTRA_DUNGEONS_RESET, dungeonResetCheck.isChecked())
                .putExtra(HostService.EXTRA_DUNGEONS_RESET_TIMER, timer);
        if (Build.VERSION.SDK_INT >= 26) startForegroundService(service); else startService(service);
        Toast.makeText(this, "Solicitando início do servidor...", Toast.LENGTH_SHORT).show();
    }

    private void stopHost() {
        Intent service = new Intent(this, HostService.class).setAction(HostService.ACTION_STOP);
        startService(service);
    }

    private void renderStatus(int status) {
        if (status == 1) {
            statusText.setText("●  SERVIDOR ATIVO · PORTA " + readInt(portField, 21050));
            statusText.setTextColor(Color.rgb(91, 220, 164));
            startButton.setEnabled(false);
            stopButton.setEnabled(true);
            portField.setEnabled(false);
        } else if (status == 2) {
            int error = ServerNative.lastError();
            String reason = error == 2 ? "A porta está ocupada ou não pôde ser aberta." : error == 1 ? "O Android não conseguiu criar o socket de rede." : "Erro nativo ao iniciar (" + error + ").";
            statusText.setText("ERRO AO INICIAR\n" + reason);
            statusText.setTextColor(Color.rgb(255, 116, 116));
            startButton.setEnabled(true);
            stopButton.setEnabled(false);
            portField.setEnabled(true);
        } else {
            statusText.setText("○  SERVIDOR PARADO");
            statusText.setTextColor(Color.rgb(194, 201, 213));
            startButton.setEnabled(true);
            stopButton.setEnabled(false);
            portField.setEnabled(true);
        }
    }

    private TextView section(String label) {
        TextView view = text(label, 12, true, Color.rgb(91, 198, 176));
        view.setPadding(0, 0, 0, dp(9));
        return view;
    }

    private EditText field(String hint, String initial, boolean numeric) {
        EditText edit = new EditText(this);
        edit.setSingleLine(true);
        edit.setText(initial);
        edit.setHint(hint);
        edit.setHintTextColor(Color.rgb(120, 131, 150));
        edit.setTextColor(Color.WHITE);
        edit.setTextSize(14);
        edit.setPadding(dp(13), dp(3), dp(13), dp(3));
        edit.setBackground(background(Color.rgb(28, 35, 49), 10));
        if (numeric) edit.setInputType(android.text.InputType.TYPE_CLASS_NUMBER);
        return edit;
    }

    private TextView text(String value, int size, boolean bold, int color) {
        TextView view = new TextView(this);
        view.setText(value);
        view.setTextSize(size);
        view.setTextColor(color);
        if (bold) view.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        return view;
    }

    private Button button(String value) {
        Button button = new Button(this);
        button.setText(value);
        button.setTextSize(13);
        button.setTextColor(Color.WHITE);
        button.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        button.setAllCaps(false);
        button.setBackground(background(Color.rgb(44, 55, 73), 12));
        button.setPadding(dp(12), dp(5), dp(12), dp(5));
        return button;
    }

    private LinearLayout card() {
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(dp(15), dp(14), dp(15), dp(14));
        layout.setBackground(background(Color.rgb(22, 28, 40), 14));
        return layout;
    }

    private GradientDrawable background(int color, int radius) {
        GradientDrawable shape = new GradientDrawable();
        shape.setColor(color);
        shape.setCornerRadius(dp(radius));
        return shape;
    }

    private LinearLayout.LayoutParams gapParams(int left, int top, int right, int bottom) {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(-1, -2);
        params.setMargins(dp(left), dp(top), dp(right), dp(bottom));
        return params;
    }

    private int readInt(EditText field, int fallback) {
        try { return Integer.parseInt(field.getText().toString().trim()); }
        catch (Exception ignored) { return fallback; }
    }

    private int dp(int value) { return (int) (value * getResources().getDisplayMetrics().density + 0.5f); }

    private String getLocalIp() {
        try {
            for (NetworkInterface network : Collections.list(NetworkInterface.getNetworkInterfaces())) {
                if (!network.isUp() || network.isLoopback()) continue;
                for (InetAddress address : Collections.list(network.getInetAddresses())) {
                    if (address instanceof Inet4Address && !address.isLoopbackAddress()) return address.getHostAddress();
                }
            }
        } catch (Exception ignored) { }
        return "IP indisponível";
    }

    private void hideKeyboard() {
        View focus = getCurrentFocus();
        if (focus == null) return;
        InputMethodManager manager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        if (manager != null) manager.hideSoftInputFromWindow(focus.getWindowToken(), 0);
        focus.clearFocus();
    }

    private void requestNotificationPermissionIfNeeded() {
        if (Build.VERSION.SDK_INT >= 33 && checkSelfPermission(Manifest.permission.POST_NOTIFICATIONS) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[]{Manifest.permission.POST_NOTIFICATIONS}, 21050);
        }
    }
}
