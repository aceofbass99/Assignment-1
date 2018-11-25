using UnityEngine;
using UnityEngine.UI;

public class Score : MonoBehaviour {

    public Transform player;
    private int scoreVal = 0;
    public Text scoreText;

    public void IncreaseScore() {
        scoreVal += 1;
        Debug.Log(scoreVal.ToString());
    }

	// Update is called once per frame
	void Update () {
        scoreText.text = scoreVal.ToString();
    }
}
