package com.zeldaonline.host;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.Handler;
import android.os.Looper;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;

public final class HostService extends Service {
    public static final String ACTION_START = "com.zeldaonline.host.START";
    public static final String ACTION_STOP = "com.zeldaonline.host.STOP";
    public static final String EXTRA_PORT = "port";
    public static final String EXTRA_DAY_SPEED = "daySpeed";
    public static final String EXTRA_NIGHT_SPEED = "nightSpeed";
    public static final String EXTRA_DUNGEONS_RESET = "dungeonsReset";
    public static final String EXTRA_DUNGEONS_RESET_TIMER = "dungeonsResetTimer";

    private static final String CHANNEL_ID = "zelda_online_host";
    private static final int NOTIFICATION_ID = 21050;
    private final Handler handler = new Handler(Looper.getMainLooper());
    private PowerManager.WakeLock wakeLock;
    private int previousStatus = -1;

    private final Runnable monitor = new Runnable() {
        @Override public void run() {
            int status = ServerNative.status();
            if (status != previousStatus) {
                previousStatus = status;
                updateNotification(statusText(status));
            }
            handler.postDelayed(this, 1000);
        }
    };

    @Override public void onCreate() {
        super.onCreate();
        createNotificationChannel();
    }

    @Override public int onStartCommand(Intent intent, int flags, int startId) {
        if (intent != null && ACTION_STOP.equals(intent.getAction())) {
            stopForeground(true);
            stopSelf();
            return START_NOT_STICKY;
        }

        startForeground(NOTIFICATION_ID, buildNotification("Iniciando servidor..."));
        if (ServerNative.status() == 1) {
            handler.removeCallbacks(monitor);
            handler.post(monitor);
            return START_STICKY;
        }

        int port = intent == null ? 21050 : intent.getIntExtra(EXTRA_PORT, 21050);
        int daySpeed = intent == null ? 160 : intent.getIntExtra(EXTRA_DAY_SPEED, 160);
        int nightSpeed = intent == null ? 320 : intent.getIntExtra(EXTRA_NIGHT_SPEED, 320);
        boolean dungeonsReset = intent == null || intent.getBooleanExtra(EXTRA_DUNGEONS_RESET, true);
        int resetTimer = intent == null ? 10 : intent.getIntExtra(EXTRA_DUNGEONS_RESET_TIMER, 10);

        try {
            File config = new File(getFilesDir(), "config.json");
            JSONObject json;
            try (InputStream input = getAssets().open("config.json");
                 ByteArrayOutputStream output = new ByteArrayOutputStream()) {
                byte[] buffer = new byte[4096];
                int read;
                while ((read = input.read(buffer)) != -1) output.write(buffer, 0, read);
                json = new JSONObject(output.toString("UTF-8"));
            }
            json.put("daySpeed", Math.max(1, daySpeed));
            json.put("nightSpeed", Math.max(1, nightSpeed));
            json.put("dungeonsReset", dungeonsReset);
            json.put("dungeonsResetTimer", Math.max(1, resetTimer));
            try (FileOutputStream output = new FileOutputStream(config, false)) {
                output.write(json.toString(2).getBytes(StandardCharsets.UTF_8));
                output.flush();
            }
            boolean started = ServerNative.start(port, config.getAbsolutePath());
            if (!started && ServerNative.status() != 1) {
                updateNotification("Não foi possível iniciar o servidor");
            }
        } catch (Exception ex) {
            updateNotification("Erro ao salvar configuração: " + ex.getMessage());
        }

        acquireWakeLock();
        handler.removeCallbacks(monitor);
        handler.post(monitor);
        return START_STICKY;
    }

    private void acquireWakeLock() {
        if (wakeLock != null && wakeLock.isHeld()) return;
        PowerManager manager = (PowerManager) getSystemService(POWER_SERVICE);
        if (manager != null) {
            wakeLock = manager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "ZeldaOnlineHost:Server");
            wakeLock.setReferenceCounted(false);
            wakeLock.acquire();
        }
    }

    private String statusText(int status) {
        if (status == 1) return "Servidor ativo · porta " + getLastPort();
        if (status == 2) {
            int error = ServerNative.lastError();
            if (error == 2) return "Falha ao abrir a porta (ocupada ou bloqueada)";
            if (error == 1) return "Falha ao criar o socket de rede";
            return "Falha ao iniciar o servidor (código " + error + ")";
        }
        return "Servidor parado";
    }

    private int getLastPort() {
        return getSharedPreferences("host", MODE_PRIVATE).getInt("port", 21050);
    }

    private void updateNotification(String message) {
        NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        if (manager != null) manager.notify(NOTIFICATION_ID, buildNotification(message));
    }

    private Notification buildNotification(String message) {
        Intent open = new Intent(this, MainActivity.class);
        PendingIntent openPending = PendingIntent.getActivity(this, 1, open,
                PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
        Intent stop = new Intent(this, HostService.class).setAction(ACTION_STOP);
        PendingIntent stopPending = PendingIntent.getService(this, 2, stop,
                PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
        Notification.Builder builder = Build.VERSION.SDK_INT >= 26
                ? new Notification.Builder(this, CHANNEL_ID)
                : new Notification.Builder(this);
        builder.setSmallIcon(android.R.drawable.stat_sys_upload_done)
                .setContentTitle("Zelda Online Host")
                .setContentText(message)
                .setContentIntent(openPending)
                .setOngoing(ServerNative.status() == 1)
                .addAction(android.R.drawable.ic_media_pause, "Parar", stopPending);
        return builder.build();
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= 26) {
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID,
                    "Servidor Zelda Online", NotificationManager.IMPORTANCE_LOW);
            channel.setDescription("Mantém o servidor multiplayer em execução");
            NotificationManager manager = getSystemService(NotificationManager.class);
            if (manager != null) manager.createNotificationChannel(channel);
        }
    }

    @Override public void onDestroy() {
        handler.removeCallbacks(monitor);
        ServerNative.stop();
        if (wakeLock != null && wakeLock.isHeld()) wakeLock.release();
        super.onDestroy();
    }

    @Override public IBinder onBind(Intent intent) { return null; }
}
