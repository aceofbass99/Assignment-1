using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class timer : MonoBehaviour {
    public Text timerText;
    private float startTime;
    private float endTime;
    private bool finished = false;
    public GameObject endLevel;
    public bool countingStarted = false;
	// Use this for initialization
	void Start () {
        timerText.text = "";
	}
    public void startCounting(){
        startTime = Time.time;
        endTime = startTime + 5 * 60-1;
        countingStarted = true;
    }
	
	// Update is called once per frame
	void Update () {
        if (!countingStarted) return;
        if (finished) return;
        float t = endTime - Time.time;
        if(t <= 0){
            finished = true;
            t = 0;
            timerText.text = "0:0";
            timerText.color = Color.red;
            endLevel.SetActive(true);
            return;
        }
        string minutes = ((int)t / 60).ToString();
        string seconds = (t % 60).ToString("f0");
        if(seconds.Length == 1){
            seconds = "0" + seconds;
        }
        timerText.text = minutes + ":" + seconds;
	}

}
