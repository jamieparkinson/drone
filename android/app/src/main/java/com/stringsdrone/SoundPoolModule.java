package com.stringsdrone;

import android.media.AudioAttributes;
import android.media.SoundPool;
import com.facebook.react.bridge.*;

import java.util.HashMap;
import java.util.Map;

public class SoundPoolModule extends ReactContextBaseJavaModule {

    private static final String E_NO_RESOURCE = "E_NO_RESOURCE";
    private static final String E_LOAD_ERROR = "E_LOAD_ERROR";

    private SoundPool pool;
    private ReactApplicationContext context;

    public SoundPoolModule(ReactApplicationContext reactContext) {
        super(reactContext);
        context = reactContext;
    }

    @ReactMethod
    public void init(ReadableMap options) {
        SoundPool.Builder poolBuilder = new SoundPool.Builder();
        AudioAttributes.Builder attributesBuilder = new AudioAttributes.Builder();

        AudioAttributes attributes = attributesBuilder
                .setUsage(options.getInt("usage"))
                .setContentType(options.getInt("contentType"))
                .build();

        pool = poolBuilder
                .setMaxStreams(options.getInt("maxStreams"))
                .setAudioAttributes(attributes)
                .build();
    }

    @ReactMethod
    public void load(String fileName, final Promise promise) {
        int res = context.getResources().getIdentifier(fileName, "raw", context.getPackageName());

        if (res == 0) {
            promise.reject(E_NO_RESOURCE, String.format("No file found with identifier '%s'", fileName));
            return;
        }

        final int soundId = pool.load(context.getApplicationContext(), res, 1);

        pool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
            private boolean wasResolved = false;

            @Override
            public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                if (wasResolved) {
                    return;
                }

                if (sampleId == soundId) {
                    if (status != 0) {
                        promise.reject(E_LOAD_ERROR, "error loading file");
                    } else {
                        WritableMap result = Arguments.createMap();
                        result.putInt("soundId", soundId);
                        promise.resolve(result);
                    }

                    wasResolved = true;
                }
            }
        });
    }

    @ReactMethod
    public void play(int soundId, ReadableMap options, Promise promise) {
        int streamId = pool.play(
            soundId,
            (float) options.getDouble("leftVolume"),
            (float) options.getDouble("rightVolume"),
            options.getInt("priority"),
            options.getInt("loop"),
            (float) options.getDouble("rate")
        );

        WritableMap result = Arguments.createMap();
        result.putInt("streamId", streamId);
        promise.resolve(result);
    }

    @ReactMethod
    public void pause(int streamId) {
        pool.pause(streamId);
    }

    @ReactMethod
    public void resume(int streamId) {
        pool.resume(streamId);
    }

    @ReactMethod
    public void stop(int streamId) {
        pool.stop(streamId);
    }

    @ReactMethod
    public void pauseAll() {
        pool.autoPause();
    }

    @ReactMethod
    public void resumeAll() {
        pool.autoResume();
    }

    @ReactMethod
    public void releasePool() {
        pool.release();
    }

    @Override
    public String getName() {
        return "SoundPool";
    }

    @Override
    public Map<String, Object> getConstants() {
        final Map<String, Object> constants = new HashMap<>();
        final Map<String, Integer> contentTypes = new HashMap<>();
        final Map<String, Integer> usages = new HashMap<>();

        contentTypes.put("UNKNOWN", AudioAttributes.CONTENT_TYPE_UNKNOWN);
        contentTypes.put("SPEECH", AudioAttributes.CONTENT_TYPE_SPEECH);
        contentTypes.put("MUSIC", AudioAttributes.CONTENT_TYPE_MUSIC);
        contentTypes.put("MOVIE", AudioAttributes.CONTENT_TYPE_MOVIE);
        contentTypes.put("SONIFICATION", AudioAttributes.CONTENT_TYPE_SONIFICATION);

        usages.put("UNKNOWN", AudioAttributes.USAGE_UNKNOWN);
        usages.put("MEDIA", AudioAttributes.USAGE_MEDIA);
        usages.put("VOICE_COMMUNICATION", AudioAttributes.USAGE_VOICE_COMMUNICATION);
        usages.put("VOICE_COMMUNICATION_SIGNALLING", AudioAttributes.USAGE_VOICE_COMMUNICATION_SIGNALLING);
        usages.put("ALARM", AudioAttributes.USAGE_ALARM);
        usages.put("NOTIFICATION", AudioAttributes.USAGE_NOTIFICATION);
        usages.put("NOTIFICATION_RINGTONE", AudioAttributes.USAGE_NOTIFICATION_RINGTONE);
        usages.put("NOTIFICATION_COMMUNICATION_REQUEST", AudioAttributes.USAGE_NOTIFICATION_COMMUNICATION_REQUEST);
        usages.put("NOTIFICATION_COMMUNICATION_INSTANT", AudioAttributes.USAGE_NOTIFICATION_COMMUNICATION_INSTANT);
        usages.put("NOTIFICATION_COMMUNICATION_DELAYED", AudioAttributes.USAGE_NOTIFICATION_COMMUNICATION_DELAYED);
        usages.put("NOTIFICATION_EVENT", AudioAttributes.USAGE_NOTIFICATION_EVENT);
        usages.put("ASSISTANCE_ACCESSIBILITY", AudioAttributes.USAGE_ASSISTANCE_ACCESSIBILITY);
        usages.put("ASSISTANCE_NAVIGATION_GUIDANCE", AudioAttributes.USAGE_ASSISTANCE_NAVIGATION_GUIDANCE);
        usages.put("ASSISTANCE_SONIFICATION", AudioAttributes.USAGE_ASSISTANCE_SONIFICATION);
        usages.put("GAME", AudioAttributes.USAGE_GAME);

        constants.put("CONTENT_TYPE", contentTypes);
        constants.put("USAGE", usages);

        return constants;
    }
}
