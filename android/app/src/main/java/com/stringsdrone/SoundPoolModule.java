package com.stringsdrone;

import android.media.AudioAttributes;
import android.media.SoundPool;
import android.util.Log;
import com.facebook.react.bridge.*;

public class SoundPoolModule extends ReactContextBaseJavaModule {

    private SoundPool pool;
    private int soundId;
    private int streamId;
    private ReactApplicationContext context;

    public SoundPoolModule(ReactApplicationContext reactContext) {
        super(reactContext);
        context = reactContext;
    }

    @Override
    public String getName() {
        return "SoundPool";
    }

    private void resolveIntegerField(int value, String key, Promise promise) {
        WritableMap result = Arguments.createMap();
        result.putInt(key, value);
        promise.resolve(result);
    }

    @ReactMethod
    public void init() {
        SoundPool.Builder poolBuilder = new SoundPool.Builder();
        AudioAttributes.Builder attributesBuilder = new AudioAttributes.Builder();

        AudioAttributes attributes = attributesBuilder
                .setUsage(AudioAttributes.USAGE_MEDIA)
                .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                .build();

        pool = poolBuilder
                .setMaxStreams(1)
                .setAudioAttributes(attributes)
                .build();
    }

    @ReactMethod
    public void load(String fileName) {
        Log.d("SoundPool", fileName);
        int res = context.getResources().getIdentifier(fileName, "raw", context.getPackageName());
        soundId = pool.load(context.getApplicationContext(), res, 1);
    }

    @ReactMethod
    public void play() {
        streamId = pool.play(soundId, 1.0f, 1.0f, 1, -1, 1.0f);
    }
}
